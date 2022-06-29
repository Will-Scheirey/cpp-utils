#pragma once
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <CL/opencl.h>

const char* getErrorString(cl_int error)
{
	switch (error) {
		// run-time and JIT compiler errors
	case 0: return "CL_SUCCESS";
	case -1: return "CL_DEVICE_NOT_FOUND";
	case -2: return "CL_DEVICE_NOT_AVAILABLE";
	case -3: return "CL_COMPILER_NOT_AVAILABLE";
	case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case -5: return "CL_OUT_OF_RESOURCES";
	case -6: return "CL_OUT_OF_HOST_MEMORY";
	case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case -8: return "CL_MEM_COPY_OVERLAP";
	case -9: return "CL_IMAGE_FORMAT_MISMATCH";
	case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case -11: return "CL_BUILD_PROGRAM_FAILURE";
	case -12: return "CL_MAP_FAILURE";
	case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case -15: return "CL_COMPILE_PROGRAM_FAILURE";
	case -16: return "CL_LINKER_NOT_AVAILABLE";
	case -17: return "CL_LINK_PROGRAM_FAILURE";
	case -18: return "CL_DEVICE_PARTITION_FAILED";
	case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

		// compile-time errors
	case -30: return "CL_INVALID_VALUE";
	case -31: return "CL_INVALID_DEVICE_TYPE";
	case -32: return "CL_INVALID_PLATFORM";
	case -33: return "CL_INVALID_DEVICE";
	case -34: return "CL_INVALID_CONTEXT";
	case -35: return "CL_INVALID_QUEUE_PROPERTIES";
	case -36: return "CL_INVALID_COMMAND_QUEUE";
	case -37: return "CL_INVALID_HOST_PTR";
	case -38: return "CL_INVALID_MEM_OBJECT";
	case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case -40: return "CL_INVALID_IMAGE_SIZE";
	case -41: return "CL_INVALID_SAMPLER";
	case -42: return "CL_INVALID_BINARY";
	case -43: return "CL_INVALID_BUILD_OPTIONS";
	case -44: return "CL_INVALID_PROGRAM";
	case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
	case -46: return "CL_INVALID_KERNEL_NAME";
	case -47: return "CL_INVALID_KERNEL_DEFINITION";
	case -48: return "CL_INVALID_KERNEL";
	case -49: return "CL_INVALID_ARG_INDEX";
	case -50: return "CL_INVALID_ARG_VALUE";
	case -51: return "CL_INVALID_ARG_SIZE";
	case -52: return "CL_INVALID_KERNEL_ARGS";
	case -53: return "CL_INVALID_WORK_DIMENSION";
	case -54: return "CL_INVALID_WORK_GROUP_SIZE";
	case -55: return "CL_INVALID_WORK_ITEM_SIZE";
	case -56: return "CL_INVALID_GLOBAL_OFFSET";
	case -57: return "CL_INVALID_EVENT_WAIT_LIST";
	case -58: return "CL_INVALID_EVENT";
	case -59: return "CL_INVALID_OPERATION";
	case -60: return "CL_INVALID_GL_OBJECT";
	case -61: return "CL_INVALID_BUFFER_SIZE";
	case -62: return "CL_INVALID_MIP_LEVEL";
	case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
	case -64: return "CL_INVALID_PROPERTY";
	case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
	case -66: return "CL_INVALID_COMPILER_OPTIONS";
	case -67: return "CL_INVALID_LINKER_OPTIONS";
	case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

		// extension errors
	case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
	case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
	case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
	case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
	case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
	case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
	default: return "Unknown OpenCL error";
	}
}

static std::string read_file(const char* fileName) {
	std::fstream f;
	f.open(fileName, std::ios_base::in);

	std::string res;
	while (!f.eof()) {
		char c;
		f.get(c);
		res += c;
	}

	f.close();

	res.pop_back();

	return std::move(res);
}

void assert_cl_success(const cl_int err, const char* message) {
	if (err) {
		std::string error_message = std::string(message);
		error_message += ".\n\tError Code: ";

		//throw std::runtime_error(error_message);
		std::cout << message << "\n\tError Code: " << err << "\n\tError Name: " << getErrorString(err) << "\n";
		exit(1);
	}
}

struct CL_Device_Info {
	cl_platform_id platform_id;
	cl_device_id device_id;
	cl_uint num_platforms;
	cl_uint num_devices;

	CL_Device_Info() {
		cl_int err;
		err = clGetPlatformIDs(1, &platform_id, &num_platforms);
		assert_cl_success(err, "Error getting OpenCL platform IDs");

		err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &num_devices);
		assert_cl_success(err, "Error getting OpenCL device IDs");
	}

};

struct CL_Buffer {
	cl_mem buffer;
	size_t size;

	CL_Buffer(cl_mem buffer, size_t size) : buffer(buffer), size(size) {};
};

class CL_Util {
public:
	CL_Util() {
		device_id = _get_device_id();

		context = _create_context();

		queue = _create_command_queue();
	}

	CL_Util(const char* filename) {
		device_id = _get_device_id();

		context = _create_context();

		queue = _create_command_queue();

		create_and_build_program(filename);
	}

	CL_Util(const char* filename, const char* function_name) {
		device_id = _get_device_id();

		context = _create_context();

		queue = _create_command_queue();

		create_program_and_kernel(filename, function_name);
	}

	// Getter methods

	CL_Device_Info   get_device_info() { return device_info; }
	cl_device_id     get_device_id()   { return device_id;   }
	cl_context       get_context()     { return context;     }
	cl_command_queue get_queue()       { return queue;       }
	cl_program       get_program()     { return program;     }
	cl_kernel        get_kernel()      { return kernel;      }

	int get_current_kernel_arg() { return current_kernel_arg; } // Too big to format

	void create_program(const char* filename) {
		program = _create_program(filename);
	}

	void build_program() {
		_build_program();
	}

	void create_and_build_program(const char* filename) {
		program = _create_program(filename);
		_build_program();
	}

	void create_kernel(const char* function_name) {
		kernel = _create_kernel(function_name);
	}

	void create_program_and_kernel(const char* filename, const char* function_name) {
		create_and_build_program(filename);
		create_kernel(function_name);
	}

	cl_mem create_raw_buffer(size_t size) {
		cl_int err;
		cl_mem buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, size, nullptr, &err);
		assert_cl_success(err, "Error creating OpenCL buffer");

		return buffer;
	}

	CL_Buffer create_buffer(size_t size) {
		cl_mem buffer = create_raw_buffer(size);

		return CL_Buffer(buffer, size);
	}

	template <typename T>
	void write_to_buffer(cl_mem buffer, size_t size, T* data) {
		_write_to_buffer(buffer, size, data);
	}

	template <typename T>
	void write_to_buffer(cl_mem buffer, T* data) {
		write_to_buffer(buffer, sizeof(data), data);
	}

	template <typename T>
	void write_to_buffer(CL_Buffer buffer, T* data) {
		write_to_buffer(buffer.buffer, buffer.size, data);
	}

	template <typename T>
	CL_Buffer create_and_write_buffer(size_t size, T* data) {
		CL_Buffer buffer = create_buffer(size);
		write_to_buffer(buffer.buffer, size, data);

		return buffer;
	}

	template <typename T>
	CL_Buffer create_and_write_buffer(T* data) {
		CL_Buffer buffer = create_buffer(sizeof(data));
		write_to_buffer(buffer.buffer, sizeof(data), data);

		return buffer;
	}

	void read_from_buffer(void* data_ptr, CL_Buffer buffer) {
		cl_int err = clEnqueueReadBuffer(queue, buffer.buffer, CL_TRUE, 0, buffer.size, data_ptr,
			0, nullptr, nullptr);
	}

	void read_from_buffer(void* data_ptr, size_t size, cl_mem buffer) {
		cl_int err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, size, data_ptr,
			0, nullptr, nullptr);
	}

	void set_kernel_arg(int index, cl_mem buffer) {
		cl_int err = clSetKernelArg(kernel, (cl_uint)index, sizeof(cl_mem), &buffer);
		assert_cl_success(err, "Error setting OpenCL kernel arg");
	}

	void set_kernel_arg(cl_mem buffer) {
		set_kernel_arg(current_kernel_arg, buffer);
		current_kernel_arg++;
	}

	void set_kernel_arg(int index, CL_Buffer buffer) {
		set_kernel_arg(index, buffer.buffer);
	}

	void set_kernel_arg(CL_Buffer buffer) {
		set_kernel_arg(buffer.buffer);
	}

	void run_kernel(size_t global_item_size, size_t local_item_size) {
		cl_int err = clEnqueueNDRangeKernel(queue, kernel, 1, 0, &global_item_size, &local_item_size,
			0, nullptr, nullptr);
		assert_cl_success(err, "Error enqueuing OpenCL kernel");
	}

	void finish_queue() {
		clFinish(queue);
	}

private:
	CL_Device_Info device_info;

	cl_device_id device_id;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;

	int current_kernel_arg = 0;

	void _assert_program_build_success(const cl_int err) {
		if (err) {
			std::cout << "Error building OpenCL program\n\tCode: " << err << "\n\n";

			cl_int info_err;

			char* log;
			size_t log_length;

			info_err = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_length);

			log = (char*)malloc(log_length);

			info_err = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_length, log, nullptr);

			assert_cl_success(info_err, "Error getting OpenCL program build info");

			std::cout << log << "\n";

			
			exit(1);
		}
	}

	cl_device_id _get_device_id() {
		device_info = CL_Device_Info();
		
		return device_info.device_id;
	}

	cl_context _create_context() {
		cl_int err;

		cl_context context = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &err);
		assert_cl_success(err, "Error creating OpenCL context");

		return context;
	}

	cl_command_queue _create_command_queue() {
		cl_int err;
		
		cl_command_queue queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);
		assert_cl_success(err, "Error creating OpenCL command queue");

		return queue;
	}

	cl_program _create_program(const char* filename) {
		cl_int err;

		std::string s = read_file(filename);
		const char* program_source = s.c_str();
		size_t source_length = 0;

		cl_program program = clCreateProgramWithSource(context, 1, &program_source, &source_length, &err);
		assert_cl_success(err, "Error creating program");

		return program;
	}

	void _build_program() {
		cl_int err = clBuildProgram(program, 1, &device_id, "", nullptr, nullptr);
		_assert_program_build_success(err);
	}

	cl_kernel _create_kernel(const char* function_name) {
		cl_int err;

		cl_kernel kernel = clCreateKernel(program, function_name, &err);
		assert_cl_success(err, "Error creating OpenCL kernel");

		return kernel;
	}

	template <typename T>
	void _write_to_buffer(cl_mem buffer, size_t size, T* data) {
		cl_int err = clEnqueueWriteBuffer(queue, buffer, CL_TRUE, 0, size, data, 0, nullptr, nullptr);
		assert_cl_success(err, "Error writing to buffer");
	}
};