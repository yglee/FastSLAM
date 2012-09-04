

#ifndef OPENCL_WRAPPER_H
#define OPENCL_WRAPPER_H

#include <OpenCL/cl.h>
#include <OpenCL/cl_gl.h>
#include <string>
#include <iostream>

// forward decl
class OpenCLContext;
class OpenCLProgram;

// defines
#define TEXTURE_TYPE_UBYTE 1
#define TEXTURE_TYPE_HALF  2
#define TEXTURE_TYPE_FLOAT 4

////////////////////////////////////////////////////////////////////////////////
// OpenCL memory location abstraction
// base class for OpenCLBuffer and OpenCLTexture

class OpenCLMemory
{
    friend class OpenCLContext;
    
public:
    ~OpenCLMemory()
    {
    }
    
protected:
    OpenCLMemory()
    : _buffer(NULL)
    {
    }
    
    cl_mem _buffer;
};

////////////////////////////////////////////////////////////////////////////////
// OpenCL memory buffer abstraction

class OpenCLBuffer : public OpenCLMemory
{
    friend class OpenCLContext;
    friend class OpenCLKernel;
    
public:
    ~OpenCLBuffer();
    
    bool IsValid() const
    {
        return _buffer != NULL;
    }
    size_t GetSize() const
    {
        return _size;
    }
    
    // upload of data to the memory buffer
    // it checks that the transfer does not exceed the bounds of the buffer
    // if blocking is false the source buffer must be kept around until the
    // transfer completes
    bool UploadData(const void *data, size_t size, size_t offset = 0,
                    bool blocking = true);
    
    // download data from the memory buffer
    // it checks that the transfer does not exceed the bounds of he buffer
    // if blocking is false the destination buffer must be kept around until
    // the transfer completes
    bool DownloadData(void *data, size_t size, size_t offset = 0,
                      bool blocking = true);
    
private:
    OpenCLBuffer(OpenCLContext &context, size_t size,
                 cl_mem_flags flags, bool create_ogl_buffer);
    
    OpenCLContext &_context;
    size_t _size;
    //unsigned int _ogl_buffer;
};

////////////////////////////////////////////////////////////////////////////////
// OpenCL kernel abstraction

class OpenCLKernel
{
    friend class OpenCLProgram;
    
public:
    ~OpenCLKernel();
    
    bool IsValid() const
    {
        return _kernel != NULL;
    }
    
    // set arguments
    template<class T>
    bool SetArg(int argNo, T arg);
    
    template<class T>
    bool SetArray(int argNo, T *arg, int numElem);
    
    bool SetBuf(int argNo, OpenCLBuffer *buf);
    
    // call the kernel
    bool RunKernel(int dim, const size_t *workSize, const size_t *localWorkSize = NULL);
    
private:
    OpenCLKernel(OpenCLContext &context, const OpenCLProgram &program,
                 const std::string &name);
    
    OpenCLContext &_context;
    const OpenCLProgram &_program;
    cl_kernel _kernel;
    std::string _name;
};

template<class T>
bool OpenCLKernel::SetArg(int argNo, T arg)
{
    if (!_kernel)
        return false;
    
    cl_int error = clSetKernelArg(_kernel, argNo, sizeof(T), &arg);
    if (error == CL_SUCCESS)
        return true;
    
    std::cerr << "Error setting kernel argument " << argNo << " : " << error << std::endl;
    std::cerr << "Arg size: " << sizeof(T) << std::endl;
    
    
    return false;
}

template<class T>
bool OpenCLKernel::SetArray(int argNo, T *arg, int numElem)
{
    if (!_kernel)
        return false;
    
    cl_int error = clSetKernelArg(_kernel, argNo, sizeof(T)*numElem, arg);
    if (error == CL_SUCCESS)
        return true;
    
    std::cerr << "Error setting kernel argument " << argNo << " : " << error << std::endl;
    std::cerr << "Arg size: " << (sizeof(T)*numElem) << std::endl;
    
    
    return false;
}


////////////////////////////////////////////////////////////////////////////////
// OpenCL program abstraction

class OpenCLProgram
{
    friend class OpenCLContext;
    friend class OpenCLKernel;
    
public:
    ~OpenCLProgram();
    
    bool IsValid() const
    {
        return _program != NULL;
    }
    
    OpenCLKernel *CreateKernel(const std::string &name);
    
private:
    OpenCLProgram(OpenCLContext &context, const std::string &code);
    
    OpenCLContext &_context;
    cl_program _program;
    std::string _code;
};

////////////////////////////////////////////////////////////////////////////////
// OpenCL context abstraction
// initializes the platform, the device and the context
// as well as a default queue

class OpenCLContext
{
    friend class OpenCLBuffer;
    friend class OpenCLTexture;
    friend class OpenCLKernel;
    friend class OpenCLProgram;
    
public:
    // the constructor initializes the OpenCL platform, device and context
    OpenCLContext(bool openGLSupport = false);
    ~OpenCLContext();
    
    // To check successful creation of context
    bool IsValid() const
    {
        return _context != NULL;
    }
    
    // Queries to capabilities of the OpenCL device
    bool HasImagingSupport() const;
    size_t MaxWorkGroupSize() const;
    size_t MaxLocalMemory() const;
    size_t MaxMemAllocSize() const;
    
    // Create programs
    OpenCLProgram *CreateProgram(const std::string &code);
    OpenCLProgram *CreateProgramFromFile(const std::string &filename);
    
    // Create mem buffer
    OpenCLBuffer *AllocMemBuffer(size_t size, cl_mem_flags flags);
    
    // Flow control
    void Finish();

private:
    cl_platform_id _platform;
    cl_device_id _device;
    cl_context _context;
    cl_command_queue _queue;
    
    bool _supportsOpenGL;
};

////////////////////////////////////////////////////////////////////////////////

#endif // OPENCL_WRAPPER_H

