#include "jit_compiler.hpp"
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <unordered_map>

namespace x64_vm {
namespace jit {

class JITCompiler::Impl {
public:
    Impl() {
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();

        auto jit = llvm::orc::LLJITBuilder().create();
        if (!jit) {
            throw std::runtime_error("Failed to create LLJIT");
        }
        jit_ = std::move(*jit);
    }

    void* compile(const std::vector<uint8_t>& code) {
        auto context = std::make_unique<llvm::LLVMContext>();
        auto module = std::make_unique<llvm::Module>("jit_module", *context);
        auto builder = std::make_unique<llvm::IRBuilder<>>(*context);

        // Create function type
        auto func_type = llvm::FunctionType::get(
            builder->getVoidTy(),
            false
        );

        // Create function
        auto func = llvm::Function::Create(
            func_type,
            llvm::Function::ExternalLinkage,
            "jit_function",
            *module
        );

        // Create basic block
        auto block = llvm::BasicBlock::Create(*context, "entry", func);
        builder->SetInsertPoint(block);

        // Add code
        for (auto byte : code) {
            // Convert byte to instruction
            // This is a simplified version - actual implementation would
            // need to properly decode x64 instructions
            builder->CreateRetVoid();
        }

        // Add module to JIT
        auto thread_safe_module = llvm::orc::ThreadSafeModule(
            std::move(module),
            std::move(context)
        );

        auto err = jit_->addIRModule(std::move(thread_safe_module));
        if (err) {
            throw std::runtime_error("Failed to add IR module");
        }

        // Look up function
        auto sym = jit_->lookup("jit_function");
        if (!sym) {
            throw std::runtime_error("Failed to look up function");
        }

        return (void*)sym->getAddress();
    }

    void free(void* compiled_code) {
        // In a real implementation, we would need to track and free
        // the compiled code. For simplicity, we'll just clear the cache.
        clear_cache();
    }

    void clear_cache() {
        jit_ = nullptr;
        auto jit = llvm::orc::LLJITBuilder().create();
        if (!jit) {
            throw std::runtime_error("Failed to create LLJIT");
        }
        jit_ = std::move(*jit);
    }

private:
    std::unique_ptr<llvm::orc::LLJIT> jit_;
};

JITCompiler::JITCompiler() : impl_(std::make_unique<Impl>()) {}
JITCompiler::~JITCompiler() = default;

void* JITCompiler::compile(const std::vector<uint8_t>& code) {
    return impl_->compile(code);
}

void JITCompiler::free(void* compiled_code) {
    impl_->free(compiled_code);
}

void JITCompiler::clear_cache() {
    impl_->clear_cache();
}

} // namespace jit
} // namespace x64_vm 