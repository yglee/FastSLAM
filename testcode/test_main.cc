#include <iostream>

#include "openCLWrapper.h"
#include <assert.h>
#include <iostream>
#include <math.h>

#define PROGRAM_FILE "vector_add_gpu.cl"

using namespace std;


const int NUM_SRC_PARAMS = 2;
const int NUM_RET_PARAMS = 1;
const int VEC_SIZE = 1234567;

size_t shrRoundUp( const size_t group_size, const size_t global_size ) 
{
    size_t r = global_size % group_size;
    if( r == 0 )
        return global_size;
    return global_size + group_size - r;
}

int main()
{
    OpenCLContext context;
    OpenCLKernel *math_kernel;
    
    //initialize context (sets up platform, device, context, command queue)
    context =  OpenCLContext(0);
    
    //get kernel paths (hard coded for now)
    string kernelpaths = "/Users/ylee8/FastSLAM/testcode";
    
    //create program (CreateProgramFromFile also builds the prog)
    OpenCLProgram *prog = context.CreateProgramFromFile(kernelpaths + PROGRAM_FILE);
    if(!prog) {
        cerr<<"Error: couldn't load OpenCL program!"<<endl;
        return 0;
    }
    
    //extract the kernel
    math_kernel = prog->CreateKernel("vecAdd_k");
    if (!math_kernel) {
        cerr<<"Error: couldn't load OpenCL Kernel!"<<endl;
    }
    
    //////////////////////////
    // allocate mem for source
    //////////////////////////
    
    //allocates a buffer of size mem_size and copies mem_size bytes from src1_dataf and src2_dataf 
    OpenCLBuffer *src_params;
    const int mem_size = VEC_SIZE * sizeof(float);
    src_params = context.AllocMemBuffer(mem_size,CL_MEM_READ_ONLY);
    
    // Initialize both vectors
    float* src1_dataf = new float[VEC_SIZE];
    float* src2_dataf = new float[VEC_SIZE];
    for (int i = 0; i < VEC_SIZE; i++) {
        src1_dataf[i] = src2_dataf[i] = (float) i;
    }
    src_params->UploadData(src1_dataf, VEC_SIZE * sizeof(float));
    src_params->UploadData(src2_dataf, VEC_SIZE * sizeof(float));

    //////////////////////////
    // allocate mem for result
    //////////////////////////
    
    OpenCLBuffer *res_params;
    res_params = context.AllocMemBuffer(mem_size,CL_MEM_WRITE_ONLY);

    
    /////////////////////
    // launching kernel
    /////////////////////
    
    //set up the kernel args (calls clKernelArgs)
    math_kernel->SetBuf(0,src_params); //SetBuf(which argument, buffer)
    math_kernel->SetBuf(1,res_params); 
    
    
    //context finish (blocks until everything in CL queue is processed
    context.Finish();
    
    //run the kernel
    const size_t global_size_offset = NULL; //
    const size_t local_work_size = 512; //total number of work items
    const size_t global_work_size = shrRoundUp(local_work_size,VEC_SIZE); //num of work items per work group
    int dim = 1;
    

    math_kernel->RunKernel(dim, &global_work_size, &local_work_size);
    context.Finish();
    return 1;
}
