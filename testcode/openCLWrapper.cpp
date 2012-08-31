
#include "openCLWrapper.h"
#include "openGLUtils.h"

#include "common/tf/stopwatch.h"

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
{CL_INVALID_GL_OBJECT, "CL_INVALID_GL_OBJECT"},
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
: _context(context), _size(size), _ogl_buffer(0) 
{
    cl_int error = 0;
    
    /*
    // this doesn't seem to be true
    size_t mmas = _context.MaxMemAllocSize();
    if (size > mmas)
    {
        cerr << "Error allocating OpenCL buffer of size " << size
             << ", size exceeds max. mem alloc size of " << mmas << endl;
        
        //return;
    }
    */
    
    if (create_ogl_buffer) {
        // clear any previous errors
        glGetError();
        
        // allocate a pixel buffer of the right size
        glGenBuffers(1, &_ogl_buffer);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, _ogl_buffer);
        glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, size, 0, GL_STREAM_DRAW_ARB);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
        
        GLenum gl_error = glGetError();
        if (gl_error != GL_NO_ERROR) {
            cerr << "Error: OpenGL error during buffer creation: "
                 << gl_error << endl;
            
            glDeleteBuffers(1, &_ogl_buffer);
            _ogl_buffer = 0;
            
            return;
        }
        
        _buffer = clCreateFromGLBuffer(_context._context, flags,
                                       _ogl_buffer, &error);
    }
    else {
        _buffer = clCreateBuffer(_context._context, flags, size, NULL, &error);
    }
    
    if (error != CL_SUCCESS) {
        cerr << "Error allocating OpenCL buffer of size " << size
             << ", error : " << GetOpenCLErrorString(error) << endl;
        
        _buffer = NULL;
        _size = 0;
        if (_ogl_buffer) {
            glDeleteBuffers(1, &_ogl_buffer);
            _ogl_buffer = 0;
        }
    }
}

OpenCLBuffer::~OpenCLBuffer() 
{
    if (_buffer)
        clReleaseMemObject(_buffer);
    if (_ogl_buffer)
        glDeleteBuffers(1, &_ogl_buffer);
}

bool OpenCLBuffer::UploadData(const void *data, size_t size, size_t offset,
                              bool blocking)
{
    cl_int error = 0;
    
    if (offset + size > _size)
        return false;
    
    OpenCLMemory *ptr = this;
    if (_ogl_buffer)
        _context.AcquireGLObjects(&ptr, 1);
    
    error = clEnqueueWriteBuffer(_context._queue, _buffer, blocking,
                                 offset, size, data, 0, 0, 0);
    
    if (_ogl_buffer)
        _context.ReleaseGLObjects(&ptr, 1);
    
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
    if (_ogl_buffer)
        _context.AcquireGLObjects(&ptr, 1);
    
    error = clEnqueueReadBuffer(_context._queue, _buffer, blocking,
                                offset, size, data, 0, 0, 0);
    
    if (_ogl_buffer)
        _context.ReleaseGLObjects(&ptr, 1);
    
    if (error != CL_SUCCESS) {
        cerr << "Error reading OpenCL buffer: "
             << GetOpenCLErrorString(error) << endl;
        return false;
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////

OpenCLTexture::OpenCLTexture(OpenCLContext &context,
                             size_t size[3], int channels, int type_size,
                             cl_mem_flags flags)
: _context(context), _channels(channels), _type_size(type_size),
  _ogl_tex(0), _io_buf(0)
{
    _size[0] = size[0];
    _size[1] = size[1];
    _size[2] = size[2];
    
    glGenTextures(1, &_ogl_tex);
    
    bool is_3d = (_size[2] != 0);
    cl_int error = 0;
    
    // determine the right texture format
    int internal_format;
    int transfer;
    if (channels == 1) {
        if (type_size == 1)
            internal_format = GL_R;
        else if (type_size == 2)
            internal_format = GL_R16F;
        else
            internal_format = GL_R32F;
        
        transfer = GL_R;
    }
    else if (channels == 2)  {
        if (type_size == 1)
            internal_format = GL_RG;
        else if (type_size == 2)
            internal_format = GL_RG16F;
        else
            internal_format = GL_RG32F;
        
        transfer = GL_RG;
    }
    else if (channels == 2) {
        if (type_size == 1)
            internal_format = GL_RGB;
        else if (type_size == 2)
            internal_format = GL_RGB16F;
        else
            internal_format = GL_RGB32F;
        
        transfer = GL_RGB;
    }
    else {
        if (type_size == 1)
            internal_format = GL_RGBA;
        else if (type_size == 2)
            internal_format = GL_RGBA16F;
        else
            internal_format = GL_RGBA32F;
        
        transfer = GL_RGBA;
    }
    
    // clear any existing errors
    glGetError();
    
    if (!is_3d) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _ogl_tex);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format,
                     _size[0], _size[1], 0, transfer, GL_FLOAT, 0);
        
        glDisable(GL_TEXTURE_2D);
        
        _buffer = clCreateFromGLTexture2D(_context._context, flags,
                                          GL_TEXTURE_2D, 0 /*mip level*/,
                                          _ogl_tex, &error);
    }
    else {
        glEnable(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D, _ogl_tex);
        glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        
        glTexImage3D(GL_TEXTURE_3D, 0, internal_format,
                     _size[0], _size[1], _size[2], 0, transfer, GL_FLOAT, 0);
        
        glDisable(GL_TEXTURE_3D);
        
        _buffer = clCreateFromGLTexture3D(_context._context, flags,
                                          GL_TEXTURE_3D, 0 /*mip level*/,
                                          _ogl_tex, &error);
    }
    
    GLenum gl_error = glGetError();
    if (gl_error != GL_NO_ERROR) {
        cerr << "Error: OpenGL error during texture creation: "
             << gl_error << endl;
        
        glDeleteTextures(1, &_ogl_tex);
        
        if (_buffer)
            clReleaseMemObject(_buffer);
        
        _buffer = NULL;
        _size[0] = 0;
        _size[1] = 0;
        _size[2] = 0;
        
        return;
    }
    
    if (error != CL_SUCCESS) {
        cerr << "Error allocating OpenCL texture of size "
             << "(" << _size[0] << ", " << _size[1] << ", " << _size[2] << ")"
             << ", error : " << GetOpenCLErrorString(error) << endl;
        
        _buffer = NULL;
        _size[0] = 0;
        _size[1] = 0;
        _size[2] = 0;
    }
    
    glGenBuffers(1, &_io_buf);
}

OpenCLTexture::OpenCLTexture(OpenCLContext &context,
                             int opengl_tex, bool is_3d,
                             cl_mem_flags flags)
: _context(context), _channels(0), _type_size(0),
  _ogl_tex(opengl_tex), _io_buf(0)
{
    cl_int error = 0;
    
    
    
    if (!is_3d) {
        _buffer = clCreateFromGLTexture2D(_context._context, flags,
                                          GL_TEXTURE_2D, 0 /*mip level*/,
                                          opengl_tex, &error);
    }
    else {
        _buffer = clCreateFromGLTexture3D(_context._context, flags,
                                          GL_TEXTURE_3D, 0 /*mip level*/,
                                          opengl_tex, &error);
    }
    
    if (error != CL_SUCCESS) {
        cerr << "Error getting OpenCL texture from OpenGL" <<
                ", error : " << GetOpenCLErrorString(error) << endl;
        _buffer = NULL;
        return;
    }
    
    if (!is_3d) {
        int tmp;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tmp);
        _size[0] = tmp;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tmp);
        _size[1] = tmp;
        _size[2] = 0;
    }
    else {
        int tmp;
        glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &tmp);
        _size[0] = tmp;
        glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_HEIGHT, &tmp);
        _size[1] = tmp;
        glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_DEPTH, &tmp);
        _size[2] = tmp;
    }
    
    glGenBuffers(1, &_io_buf);
}

OpenCLTexture::~OpenCLTexture()
{
    if (_buffer)
        clReleaseMemObject(_buffer);
    
    // only delete the OpenGL textrue if we created it
    if (_type_size != 0)
        glDeleteTextures(1, &_ogl_tex);
    
    if (_io_buf != 0)
        glDeleteBuffers(1, &_io_buf);
}

bool OpenCLTexture::TransferDataToImage(OpenCLBuffer *data, size_t data_offset,
                                        size_t size[3], size_t offset[3],
                                        bool blocking)
{
    if (offset[0] + size[0] > _size[0] ||
        offset[1] + size[1] > _size[1] ||
        offset[2] + size[2] > _size[2]) {
        cerr << "Warning: TransferDataToImage out of bounds" << endl;
        return false;
    }
    
    if (size[0] == 0 ||
        size[1] == 0 ||
        size[2] == 0) {
        cerr << "Warning: TransferDataToImage has zero size" << endl;
        return false;
    }
    
    OpenCLMemory *ptr = this;
    _context.AcquireGLObjects(&ptr, 1);
    
    cl_int error = 0;
    error = clEnqueueCopyBufferToImage(_context._queue, data->_buffer, _buffer,
                                       data_offset, offset, size,
                                       0, 0, 0);
    
    _context.ReleaseGLObjects(&ptr, 1);
    
    if (error != CL_SUCCESS) {
        cerr << "Error uploading to OpenCL texture: "
             << GetOpenCLErrorString(error) << endl;
        return false;
    }
    
    if (blocking)
        _context.Finish();
    
    return true;
}

bool OpenCLTexture::TransferDataFromImage(OpenCLBuffer *data,
                                          size_t data_offset,
                                          size_t size[3], size_t offset[3],
                                          bool blocking)
{
    if (offset[0] + size[0] > _size[0] ||
        offset[1] + size[1] > _size[1] ||
        offset[2] + size[2] > _size[2]) {
        cerr << "Warning: TransferDataFromImage out of bounds" << endl;
        return false;
    }
    
    if (size[0] == 0 ||
        size[1] == 0 ||
        size[2] == 0) {
        cerr << "Warning: TransferDataFromImage has zero size" << endl;
        return false;
    }
    
    OpenCLMemory *ptr = this;
    _context.AcquireGLObjects(&ptr, 1);
    
    cl_int error = 0;
    error = clEnqueueCopyImageToBuffer(_context._queue, _buffer, data->_buffer,
                                       offset, size, data_offset,
                                       0, 0, 0);
    
    _context.ReleaseGLObjects(&ptr, 1);
    
    if (error != CL_SUCCESS) {
        cerr << "Error uploading to OpenCL texture: "
             << GetOpenCLErrorString(error) << endl;
        return false;
    }
    
    if (blocking)
        _context.Finish();
    
    return true;
}

bool OpenCLTexture::UploadData(const void *data, size_t size[3],
                               size_t offset[3], int channels, int type_size)
{
    
    if (offset[0] + size[0] > _size[0] ||
        offset[1] + size[1] > _size[1] ||
        offset[2] + size[2] > _size[2]) {
        cerr << "Warning: UploadData out of bounds" << endl;
        cerr << "Texture size: "
             << _size[0] << " x " << _size[1] << " x " << _size[2] << endl;
        cerr << "Offset: "
             << offset[0] << " x " << offset[1] << " x " << offset[2] << endl;
        cerr << "Upload size: "
             << size[0] << " x " << size[1] << " x " << size[2] << endl;
        return false;
    }
    
    if (size[0] == 0 ||
        size[1] == 0 ||
        size[2] == 0) {
        cerr << "Warning: UploadData has zero size" << endl;
        return false;
    }
    
    int format;
    int transfer;
    if (type_size == 1)
        format = GL_UNSIGNED_BYTE;
    else if (type_size == 2)
        format = GL_HALF_FLOAT_ARB;
    else
        format = GL_FLOAT;
        
    if (channels == 1)
        transfer = GL_R;
    else if (channels == 2)
        transfer = GL_RG;
    else if (channels == 2)
        transfer = GL_RGB;
    else
        transfer = GL_RGBA;
    
    size_t bytes = size[0] * size[1] * size[2] * channels * type_size;
    
    // clear any existing errors
    glGetError();
    
    if (_size[2] == 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _ogl_tex);
        
        glTexSubImage2D(GL_TEXTURE_2D, 0, offset[0], offset[1],
                        size[0], size[1], transfer, format, data);
        
        glDisable(GL_TEXTURE_2D);
    } else {
        glEnable(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D, _ogl_tex);
        
        // using the pbuffer is way faster than doing it straight up
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, _io_buf);
        glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, bytes, NULL, GL_STREAM_DRAW);
        
        void *io_mem = glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY);
        assert(io_mem);
        memcpy(io_mem, data, bytes);
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);
        
        glTexSubImage3D(GL_TEXTURE_3D, 0, offset[0], offset[1], offset[2],
                        size[0], size[1], size[2], transfer, format, 0);
        
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
        
        glDisable(GL_TEXTURE_3D);
    }
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Error: OpenGL error during texture upload: " << error << endl;
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

bool OpenCLKernel::SetTexture(int argNo, OpenCLTexture *tex)
{
    if (!_kernel)
        return false;
    
    cl_int error = clSetKernelArg(_kernel, argNo,
                                  sizeof(cl_mem), &tex->_buffer);
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

    // make sure we have an OpenGL context before we try to allocate a OpenCL
    // context that supports it
    if (_supportsOpenGL &&
       (glXGetCurrentContext() == NULL || glXGetCurrentDisplay() == NULL)) {
        cerr << "Error getting OpenCL context (no OpenGL context)" << endl;
        
        _device = 0;
        _platform = 0;
        
        return;
    }
    
    cl_context_properties gl_props[] = {
        CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
        CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
        CL_CONTEXT_PLATFORM, (cl_context_properties)(_platform),
        0
    };

    _context = clCreateContext((_supportsOpenGL ? gl_props : props), 1,
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

OpenCLBuffer *OpenCLContext::AllocGLMemBuffer(size_t size, cl_mem_flags flags)
{
    if (!_context)
        return NULL;
    
    OpenCLBuffer *buf = new OpenCLBuffer(*this, size, flags, true);
    
    if (buf->IsValid())
        return buf;
    
    delete buf;
    
    return NULL;
}

OpenCLTexture *OpenCLContext::AllocTexture(size_t size[3], int channels,
                                           int type_size, cl_mem_flags flags)
{
    if (!_context)
        return NULL;
    
    OpenCLTexture *tex = new OpenCLTexture(*this, size, channels,
                                           type_size, flags);
    
    if (tex->IsValid())
        return tex;
    
    delete tex;
    
    return NULL;
}

OpenCLTexture *OpenCLContext::GetTextureFromGL(unsigned int ogl_tex,
                                               bool is_3d,
                                               cl_mem_flags flags)
{
    if (!_context)
        return NULL;
    
    OpenCLTexture *tex = new OpenCLTexture(*this, ogl_tex, is_3d, flags);
    
    if (tex->IsValid())
        return tex;
    
    delete tex;
    
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

bool OpenCLContext::AcquireGLObjects(OpenCLMemory **objs, int num_objs)
{
    vector<cl_mem> memobj;
    
    memobj.resize(num_objs);
    for (int i = 0; i < num_objs; ++i)
        memobj[i] = objs[i]->_buffer;
    
    cl_int error = clEnqueueAcquireGLObjects(_queue, num_objs, &memobj[0],
                                             0, NULL, NULL);
    if (error != CL_SUCCESS) {
        cerr << "Error AcquireGLObjects: "
             << GetOpenCLErrorString(error) << endl;
        return false;
    }
    
    return true;
}

void OpenCLContext::ReleaseGLObjects(OpenCLMemory **objs, int num_objs)
{
    vector<cl_mem> memobj;
    
    memobj.resize(num_objs);
    for (int i = 0; i < num_objs; ++i)
        memobj[i] = objs[i]->_buffer;
    
    cl_int error = clEnqueueReleaseGLObjects(_queue, num_objs, &memobj[0],
                                             0, NULL, NULL);
    if (error != CL_SUCCESS) {
        cerr << "Error ReleaseGLObjects: "
             << GetOpenCLErrorString(error) << endl;
        return;
    }
    
    return;
}


