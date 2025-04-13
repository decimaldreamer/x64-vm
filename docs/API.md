# x64 Virtual Machine API Documentation

## Core API

### VirtualMachine Class
- `VirtualMachine()`: Creates a virtual machine instance
- `allocate_memory(size_t size)`: Allocates memory
- `load_code(const std::vector<uint8_t>& code)`: Loads code
- `run()`: Runs the virtual machine

### Memory Management
- `VirtualMemory`: Virtual memory management
- `MemoryPool`: Memory pool management
- `StackManager`: Stack management

## Optimization API

### PerformanceOptimizer Class
- `collect_metrics()`: Collects performance metrics
- `suggest_optimizations()`: Provides optimization suggestions
- `apply_optimizations()`: Applies automatic optimizations
- `generate_performance_report()`: Generates performance report

## Security API

### SecurityManager Class
- `encrypt_memory()`: Encrypts memory
- `decrypt_memory()`: Decrypts memory
- `audit_security()`: Performs security audit 