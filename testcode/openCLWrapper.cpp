
#include "openCLWrapper.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <assert.h>
#include <memory.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////

struct ErrorMsg 
{
    cl_int code;
    const char *msg;
};

ErrorMsg openCL_error_msgs[] = {
{CL_SUCCESS, "CL_SUCCESS"},
{CL_DEVICE_NOT_FOUND, "CL_DEVICE_NOT_FOUND"},
{CL_DEVICE_NOT_AVAILABLE, "CL_DEVICE_NOT_AVAILABLE"},
{CL_COMPILER_NOT_AVAILABLE, "CL_COMPILER_NOT_AVAILABLE"},
{CL_MEM_OBJECT_ALLOCATION_FAILURE, "CL_MEM_OBJECT_ALLOCATION_FAILURE"},
{CL_OUT_OF_RESOURCES, "CL_OUT_OF_RESOURCES"},
{CL_OUT_OF_HOST_MEMORY, "CL_OUT_OF_HOST_MEMORY"},
{CL_PROFILING_INFO_NOT_AVAILABLE, "CL_PROFILING_INFO_NOT_AVAILABLE"},
{CL_MEM_COPY_OVERLAP, "CL_MEM_COPY_OVERLAP"},
{CL_IMAGE_FORMAT_MISMATCH, "CL_IMAGE_FORMAT_MISMATCH"},
{CL_IMAGE_FORMAT_NOT_SUPPORTED, "CL_IMAGE_FORMAT_NOT_SUPPORTED"},
{CL_BUILD_PROGRAM_FAILURE, "CL_BUILD_PROGRAM_FAILURE"},
{CL_MAP_FAILURE, "CL_MAP_FAILURE"},
{CL_MISALIGNED_SUB_BUFFER_OFFSET, "CL_MISALIGNED_SUB_BUFFER_OFFSET"},
{CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST,
                                "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"},

{CL_INVALID_VALUE, "CL_INVALID_VALUE"},
{CL_INVALID_DEVICE_TYPE, "CL_INVALID_DEVICE_TYPE"},
{CL_INVALID_PLATFORM, "CL_INVALID_PLATFORM"},
{CL_INVALID_DEVICE, "CL_INVALID_DEVICE"},
{CL_INVALID_CONTEXT, "CL_INVALID_CONTEXT"},
{CL_INVALID_QUEUE_PROPERTIES, "CL_INVALID_QUEUE_PROPERTIES"},
{CL_INVALID_COMMAND_QUEUE, "CL_INVALID_COMMAND_QUEUE"},
{CL_INVALID_HOST_PTR, "CL_INVALID_HOST_PTR"},
{CL_INVALID_MEM_OBJECT, "CL_INVALID_MEM_OBJECT"},
{CL_INVALID_IMAGE_FORMAT_DESCRIPTOR, "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"},
{CL_INVALID_IMAGE_SIZE, "CL_INVALID_IMAGE_SIZE"},
{CL_INVALID_SAMPLER, "CL_INVALID_SAMPLER"},
{CL_INVALID_BINARY, "CL_INVALID_BINARY"},
{CL_INVALID_BUILD_OPTIONS, "CL_INVALID_BUILD_OPTIONS"},
{CL_INVALID_PROGRAM, "CL_INVALID_PROGRAM"},
{CL_INVALID_PROGRAM_EXECUTABLE, "CL_INVALID_PROGRAM_EXECUTABLE"},
{CL_INVALID_KERNEL_NAME, "CL_INVALID_KERNEL_NAME"},
{CL_INVALID_KERNEL_DEFINITION, "CL_INVALID_KERNEL_DEFINITION"},
{CL_INVALID_KERNEL, "CL_INVALID_KERNEL"},
{CL_INVALID_ARG_INDEX, "CL_INVALID_ARG_INDEX"},
{CL_INVALID_ARG_VALUE, "CL_INVALID_ARG_VALU"},
{CL_INVALID_ARG_SIZE, "CL_INVALID_ARG_SIZE"},
{CL_INVALID_KERNEL_ARGS, "CL_INVALID_KERNEL_ARGS"},
{CL_INVALID_WORK_DIMENSION, "CL_INVALID_WORK_DIMENSION"},
{CL_INVALID_WORK_GROUP_SIZE, "CL_INVALID_WORK_GROUP_SIZE"},
{CL_INVALID_WORK_ITEM_SIZE, "CL_INVALID_WORK_ITEM_SIZE"},
{CL_INVALID_GLOBAL_OFFSET, "CL_INVALID_GLOBAL_OFFSET"},
{CL_INVALID_EVENT_WAIT_LIST, "CL_INVALID_EVENT_WAIT_LIST"},
{CL_INVALID_EVENT, "CL_INVALID_EVENT"},
{CL_INVALID_OPERATION, "CL_INVALID_OPERATION"},
{CL_INVALID_BUFFER_SIZE, "CL_INVALID_BUFFER_SIZE"},
{CL_INVALID_MIP_LEVEL, "CL_INVALID_MIP_LEVEL"},
{CL_INVALID_GLOBAL_WORK_SIZE, "CL_INVALID_GLOBAL_WORK_SIZE"},
{CL_INVALID_PROPERTY, "CL_INVALID_PROPERTY"},

{0, NULL} // end marker
};

const char *GetOpenCLErrorString(cl_int error) 
{
    int idx = 0;
    
    while (openCL_error_msgs[idx].msg != NULL) {
        if (openCL_error_msgs[idx].code == error)
            return openCL_error_msgs[idx].msg;
        
        ++idx;
    }
    
    cerr << "Warning: encountered unknown OpenCL error code: " << error << endl;
    
    return "Unknown OpenCL error";
}

////////////////////////////////////////////////////////////////////////////////

OpenCLBuffer::OpenCLBuffer(OpenCLContext &context,
                           size_t size,
                           cl_mem_flags flags,
                           bool create_ogl_buffer)
: _context(context), _size(size) 
{
    cl_int error = 0;
    

    _buffer = clCreateBuffer(_context._context, flags, size, NULL, &error);
    
    if (error != CL_SUCCESS) {
        cerr << "Error allocating OpenCL buffer of size " << size
             << ", error : " << GetOpenCLErrorString(error) << endl;
        
        _buffer = NULL;
        _size = 0;
    }
}

OpenCLBuffer::~OpenCLBuffer() 
{
    if (_buffer)
        clReleaseMemObject(_buffer);
}

bool OpenCLBuffer::UploadData(const void *data, size_t size, size_t offset,
                              bool blocking)
{
    cl_int error = 0;
    
    if (offset + size > _size)
        return false;
    
    OpenCLMemory *ptr = this;
    
    error = clEnqueueWriteBuffer(_context._queue, _buffer, blocking,
                                 offset, size, data, 0, 0, 0);
    
    if (error != CL_SUCCESS) {
        cerr << "Error writing OpenCL buffer: "
             << GetOpenCLErrorString(error) << endl;
        return false;
    }
    
    return true;
}

bool OpenCLBuffer::DownloadData(void *data, size_t size, size_t offset,
                                bool blocking)
{
    cl_int error = 0;
    
    if (offset + size > _size)
        return false;
    
    OpenCLMemory *ptr = this;
    
    error = clEnqueueReadBuffer(_context._queue, _buffer, blocking,
                                offset, size, data, 0, 0, 0);
    
    if (error != CL_SUCCESS) {
        cerr << "Error reading OpenCL buffer: "
             << GetOpenCLErrorString(error) << endl;
        return false;
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////

OpenCLKernel::~OpenCLKernel()
{
    if (_kernel)
        clReleaseKernel(_kernel);
}

OpenCLKernel::OpenCLKernel(OpenCLContext &context, const OpenCLProgram &program,
                           const std::string &name)
: _context(context), _program(program), _kernel(NULL), _name(name)
{
    cl_int error = 0;
    
    // Extract the kernel
    _kernel = clCreateKernel(_program._program, _name.c_str(), &error);
    if (error != CL_SUCCESS) {
        cerr << "Error creating OpenCL kernel ('" << _name << "' : "
             << GetOpenCLErrorString(error) << endl;
        
        _kernel = NULL;
        
        return;
    }
}

bool OpenCLKernel::SetBuf(int argNo, OpenCLBuffer *buf)
{
    if (!_kernel)
        return false;
    
    cl_int error = clSetKernelArg(_kernel, argNo,
                                  sizeof(cl_mem), &buf->_buffer);
    if (error == CL_SUCCESS)
        return true;
    
    std::cerr << "Error setting kernel argument: "
              << GetOpenCLErrorString(error) << std::endl;
    
    return false;
}

bool OpenCLKernel::RunKernel(int dim, const size_t *workSize,
                             const size_t *localWorkSize)
{
    if (!_kernel)
        return false;
    
    cl_int error = clEnqueueNDRangeKernel(_context._queue, _kernel,
                                          dim, NULL, workSize, localWorkSize,
                                          0, NULL, NULL);
    
    if (error == CL_SUCCESS)
        return true;
    
    std::cerr << "Error running kernel: "
              << GetOpenCLErrorString(error) << std::endl;
    
    return false;
}

////////////////////////////////////////////////////////////////////////////////


OpenCLProgram::OpenCLProgram(OpenCLContext &context, const std::string &code)
: _context(context), _program(NULL)
{
    cl_int error = 0;
    
    // Create the program
    size_t src_size = code.size();
    const char *cptr = code.c_str();
    _program = clCreateProgramWithSource(_context._context, 1,
                                         &cptr, &src_size, &error);
    if (error != CL_SUCCESS) {
        cerr << "Error creating OpenCL program: "
             << GetOpenCLErrorString(error) << endl;
        _program = NULL;
        return;
    }
  
    // Builds the program
    error = clBuildProgram(_program, 1, &_context._device, NULL, NULL, NULL);

    if (error != CL_SUCCESS) {
        cerr << "Error building OpenCL program: "
             << GetOpenCLErrorString(error) << endl;
        
        // Shows the log
        char* build_log;
        size_t log_size;
        // First call to know the proper size
        clGetProgramBuildInfo(_program, _context._device,
                              CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        build_log = new char[log_size+1];

        // Second call to get the log
        clGetProgramBuildInfo(_program, _context._device,
                              CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
        build_log[log_size] = '\0';
        cerr << build_log << endl;
        delete [] build_log;
        
        clReleaseProgram(_program);
        _program = NULL;
        
        return;
    }
    
    _code = code;
}

OpenCLProgram::~OpenCLProgram()
{
    if (_program)
        clReleaseProgram(_program);
}

OpenCLKernel *OpenCLProgram::CreateKernel(const std::string &name)
{
    if (!_program)
        return NULL;
    
    OpenCLKernel *kernel = new OpenCLKernel(_context, *this, name);
    if (kernel->IsValid())
        return kernel;
    
    delete kernel;
    
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////

OpenCLContext::OpenCLContext(bool openGLSupport)
: _platform(0), _device(0), _context(0), _queue(0),
  _supportsOpenGL(openGLSupport)
{
    cl_int error = 0;

    // Platform
    error = clGetPlatformIDs(1, &_platform, 0);
    if (error != CL_SUCCESS) {
        cerr << "Error getting platform id: "
             << GetOpenCLErrorString(error) << endl;
        
        _platform = 0;
        
        return;
    }

    // Device
    error = clGetDeviceIDs(_platform, CL_DEVICE_TYPE_GPU, 1, &_device, NULL);
    if (error != CL_SUCCESS) {
        cerr << "Error getting device ids: "
             << GetOpenCLErrorString(error) << endl;
        
        _device = 0;
        _platform = 0;
        
        return;
    }

    // Context
    cl_context_properties props[] = {
        CL_CONTEXT_PLATFORM, (cl_context_properties)(_platform),
        0
    };

    _context = clCreateContext(props, 1,
                               &_device, NULL, NULL, &error);
    if (error != CL_SUCCESS) {
        cerr << "Error creating context: "
             << GetOpenCLErrorString(error) << endl;
        
        _context = 0;
        _device = 0;
        _platform = 0;
        
        return;
    }

    // Default command-queue
    _queue = clCreateCommandQueue(_context, _device, 0, &error);
    if (error != CL_SUCCESS) {
        cerr << "Error creating command queue: "
             << GetOpenCLErrorString(error) << endl;
        
        // release allocated context
        clReleaseContext(_context);
        
        _queue = 0;
        _context = 0;
        _device = 0;
        _platform = 0;
        
        return;
    }
}

OpenCLContext::~OpenCLContext()
{
    if (_queue)
        clReleaseCommandQueue(_queue);
    if (_context)
        clReleaseContext(_context);
    
    _device = 0;
    _platform = 0;
}

bool OpenCLContext::HasImagingSupport() const
{
    if (!_context)
        return false;
    
    cl_bool bImageSupport;
    clGetDeviceInfo(_device, CL_DEVICE_IMAGE_SUPPORT, sizeof(bImageSupport),
                    &bImageSupport, NULL);
    return bImageSupport;
}

size_t OpenCLContext::MaxWorkGroupSize() const
{
    if (!_context)
        return 0;
    
    size_t mwgs = 0;
    clGetDeviceInfo(_device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t),
                    &mwgs, NULL);
    return mwgs;
}

size_t OpenCLContext::MaxLocalMemory() const
{
    if (!_context)
        return 0;
    
    cl_ulong mlm = 0;
    clGetDeviceInfo(_device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong),
                    &mlm, NULL);
    return mlm;
}

OpenCLProgram *OpenCLContext::CreateProgram(const std::string &code)
{
    if (!_context)
        return NULL;
    
    OpenCLProgram *prog = new OpenCLProgram(*this, code);
    if (prog->IsValid())
        return prog;
    
    delete prog;
    
    return NULL;
}

OpenCLProgram *OpenCLContext::CreateProgramFromFile(const std::string &filename)
{
    // read contents of text file into string
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cerr << "Error loading program from file: " << filename << endl;
        return NULL;
    }
    
    string code;
    
    // find the size of the data in the file and
    // pre-allocate space in the string
    file.seekg(0, std::ios::end);
    code.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    
    code.assign((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
    
    return CreateProgram(code);
}

size_t OpenCLContext::MaxMemAllocSize() const
{
    if (!_context)
        return 0;
    
    cl_ulong bsize = 0;
    clGetDeviceInfo(_device, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong),
                    &bsize, NULL);
    return bsize;
}

OpenCLBuffer *OpenCLContext::AllocMemBuffer(size_t size, cl_mem_flags flags)
{
    if (!_context)
        return NULL;
    
    OpenCLBuffer *buf = new OpenCLBuffer(*this, size, flags, false);
    
    if (buf->IsValid())
        return buf;
    
    delete buf;
    
    return NULL;
}

void OpenCLContext::Finish()
{
    if (!_context)
        return;
    
    cl_int error = 0;
    
    error = clFinish(_queue);
    if (error != CL_SUCCESS)
    {
        cerr << "Error Finish: " << GetOpenCLErrorString(error) << endl;
    }
}

