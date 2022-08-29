#pragma once
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>

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

class CL_Device_Info {
public:
	cl_int err;
	cl_platform_id platform_id;
	cl_device_id device_id;

	const char* platform_attr_names[5] = { "Name", "Vendor", "Version", "Profile", "Extensions" };
	const cl_platform_info platform_attr_types[5] = { CL_PLATFORM_NAME, CL_PLATFORM_VENDOR,
		CL_PLATFORM_VERSION, CL_PLATFORM_PROFILE, CL_PLATFORM_EXTENSIONS };

	const char* device_attr_names[4] = { "Name", "Device Version", "Driver Version", "Parallel Computing Units" };
	const cl_device_info device_attr_types[4] = { CL_DEVICE_NAME, CL_DEVICE_VERSION,
		CL_DRIVER_VERSION, CL_DEVICE_MAX_COMPUTE_UNITS};

	cl_uint get_num_platforms() {
		cl_uint num_platforms;
		err = clGetPlatformIDs(0, NULL, &num_platforms);
		assert_cl_success(err, "Error getting number of OpenCL platforms");

		return num_platforms;
	}

	cl_platform_id* get_platforms() {
		cl_uint num_platforms = get_num_platforms();
		cl_platform_id* platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * num_platforms);

		err = clGetPlatformIDs(num_platforms, platforms, NULL);
		assert_cl_success(err, "Error getting OpenCL platform IDs");
		return platforms;
	}

	cl_uint get_num_devices(cl_platform_id platform_id) {
		cl_uint num_devices;

		err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
		assert_cl_success(err, "Error getting number of OpenCL platform devices");
		return num_devices;
	}

	cl_device_id* get_devices(cl_platform_id platform_id) {
		cl_uint num_devices = get_num_devices(platform_id);
		cl_device_id* devices = (cl_device_id*)malloc(sizeof(cl_device_id) * num_devices);

		err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
		assert_cl_success(err, "Error getting OpenCL device IDs");
		return devices;
	}

	void print_platform_information(cl_platform_id platform, int i) {
		std::cout << "Platform " << i << "\n";
		char* info;
		size_t info_size;
		for (int i = 0; i < 3; i++) {
			err = clGetPlatformInfo(platform, platform_attr_types[i], 0, NULL, &info_size);
			assert_cl_success(err, "Error getting OpenCL platform info.");
			
			info = (char*)malloc(info_size);

			clGetPlatformInfo(platform, platform_attr_types[i], info_size, info, NULL);
			std::cout << "\t" << platform_attr_names[i] << ": " << info << "\n";
			free(info);
		}
	}

	void print_device_information(cl_device_id device, int i) {
		std::cout << "\n\tDevice " << i << "\n";
		char* info;
		size_t info_size;
		int l = sizeof(device_attr_types) / sizeof(*device_attr_types) - 1;
		for (int i = 0; i < l; i++) {
			err = clGetDeviceInfo(device, device_attr_types[i], 0, NULL, &info_size);
			assert_cl_success(err, "Error getting OpenCL device info.");

			info = (char*)malloc(info_size);

			clGetDeviceInfo(device, device_attr_types[i], info_size, info, NULL);
			std::cout << "\t\t" << device_attr_names[i] << ": " << info << "\n";
			free(info);
		}

		cl_uint max_compute_units;
		err = clGetDeviceInfo(device, device_attr_types[l], -1, &max_compute_units, NULL);
		assert_cl_success(err, "Error getting OpenCL device info.");
		std::cout << "\t\t" << device_attr_names[l] << ": " << max_compute_units << "\n";

		cl_uint max_clock_freq;
		err = clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, -1, &max_clock_freq, NULL);
		assert_cl_success(err, "Error getting OpenCL device info.");
		std::cout << "\t\tMax Clock Frequency: " << max_clock_freq << "MHz\n";
	}

	void print_hardware_information() {
		std::cout << "Information found: \n\n";
		for (int i = 0; i < (int)num_platforms; i++) {
			print_platform_information(platforms[i], i);
			for (int j = 0; j < (int)num_devices[i]; j++) {
				print_device_information(devices[i][j], j);
			}
		}
	}

	CL_Device_Info() {
		platforms = get_platforms();
		num_platforms = get_num_platforms();

		num_devices = (cl_uint*)malloc(sizeof(cl_uint) * num_platforms);
		for (int i = 0; i < (int)num_platforms; i++) {
			num_devices[i] = get_num_devices(platforms[i]);
			devices.push_back(get_devices(platforms[i]));
		}
		print_hardware_information();
		platform_id = platforms[0];
		device_id = devices[0][0];
	}

private:
	cl_uint num_platforms;
	cl_platform_id* platforms;
	cl_uint* num_devices;
	std::vector<cl_device_id*> devices;

};

struct CL_Buffer {
	cl_mem buffer;
	size_t size;

	CL_Buffer(cl_mem buffer, size_t size) : buffer(buffer), size(size) {};
};

class CL_Util {
public:
	CL_Util() {
		init();
	}

	CL_Util(const char* filename) {
		init();

		create_and_build_program(filename);
	}

	CL_Util(const char* filename, const char* function_name) {
		init();

		create_program_and_kernel(filename, function_name);
	}

	CL_Util(const char* filename, const char* function_name, size_t global_size, size_t local_size)
	
	{
		init();

		global_item_size = global_size;
		local_item_size = local_size;


		create_program_and_kernel(filename, function_name);
	}

	// Getter methods

	CL_Device_Info   get_device_info() { return device_info; }
	cl_device_id     get_device_id()   { return device_id;   }
	cl_context       get_context()     { return context;     }
	cl_command_queue get_queue()       { return queue;       }
	cl_program       get_program()     { return program;     }
	cl_kernel        get_kernel()      { return kernel;      }
	size_t get_global_item_size() { return global_item_size;   }
	size_t get_local_item_size()  { return local_item_size;    }
	int get_current_kernel_arg()  { return current_kernel_arg; }

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

	void set_global_item_size(size_t global_size) {
		global_item_size = global_size;
	}

	void set_local_item_size(size_t local_size) {
		local_item_size = local_size;
	}

	void set_item_sizes(size_t global_size, size_t local_size) {
		set_global_item_size(global_size);
		set_local_item_size(local_size);
	}

	void run_kernel(size_t global_size, size_t local_size) {
		set_item_sizes(global_size, local_size);
		run_kernel();
	}

	void run_kernel() {
		
		if (global_item_size <= 0) {
			std::cout << "Global item size not initialized!" << "\n";
			exit(1);
		}
		if (local_item_size <= 0) {
			std::cout << "Local item size not initialized!" << "\n";
			exit(1);
		}
		if (global_item_size % local_item_size != 0) {
			std::cout << "Global item size must be divisible by local item size!" << "\n\n";
		}
		cl_int err = clEnqueueNDRangeKernel(queue, kernel, 1, 0, &global_item_size, &local_item_size,
			0, nullptr, nullptr);
		assert_cl_success(err, "Error enqueuing OpenCL kernel");
	}

	void finish_queue() {
		clFinish(queue);
		current_kernel_arg = 0;
	}

private:
	CL_Device_Info device_info;

	cl_device_id device_id;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	size_t global_item_size;
	size_t local_item_size;

	int current_kernel_arg = 0;

	void init() {
		device_id = _get_device_id();

		context = _create_context();

		queue = _create_command_queue();
	}

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
		//device_info = CL_Device_Info();
		
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