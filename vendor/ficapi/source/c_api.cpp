// ficapi��
// The Shrug License

// Copyright � 2018 ThePhD. All rights can sometimes maybe not unreserved, yo.

#include <ficapi/c_api.hpp>

#include <cstdlib>

extern "C" struct ficapi_opaque_ {
	int value;
};

static struct ficapi_opaque_ opaque_data = ficapi_opaque_{ 2 };
static int dynamic_value = 56;

void ficapi_set_data(int v) {
	opaque_data.value = v;
}

int ficapi_get_data() {
	return opaque_data.value;
}

void ficapi_set_dynamic_data(int v) {
	dynamic_value = v;
}

int ficapi_get_dynamic_data() {
	return dynamic_value;
}

void ficapi_create(void** p_handle, ficapi_type ft) {
	switch (ft) {
	case ficapi_type::ficapi_type_int: {
		int** i_handle = static_cast<int**>(static_cast<void*>(p_handle));
		ficapi_int_create(i_handle);
		return;
	}
	case ficapi_type::ficapi_type_opaque: {
		ficapi_opaque_handle* h_handle = static_cast<ficapi_opaque_handle*>(static_cast<void*>(p_handle));
		ficapi_handle_create(h_handle);
		return;
	}
	}
}

void ficapi_re_create(void** p_handle, ficapi_type ft) {
	if (*p_handle != nullptr) {
		ficapi_delete(*p_handle, ft);
	}
	ficapi_create(p_handle, ft);
}

void ficapi_delete(void* handle, ficapi_type ft) {
	switch (ft) {
	case ficapi_type::ficapi_type_int: {
		int* i_handle = static_cast<int*>(static_cast<void*>(handle));
		ficapi_int_delete(i_handle);
		return;
	}
	case ficapi_type::ficapi_type_opaque: {
		ficapi_opaque_handle h_handle = static_cast<ficapi_opaque_handle>(handle);
		ficapi_handle_delete(h_handle);
		return;
	}
	}
}

void ficapi_int_create(int** p_handle) {
	int*& handle = *p_handle;
	handle = static_cast<int*>(malloc(sizeof(int)));
	int* ihandle = static_cast<int*>(handle);
	*ihandle = ficapi_get_dynamic_data();
}

void ficapi_int_re_create(int** p_handle) {
	if (*p_handle != nullptr) {
		ficapi_int_delete(*p_handle);
	}
	ficapi_int_create(p_handle);
}

void ficapi_int_delete(int* handle) {
	free(handle);
}

void ficapi_handle_create(ficapi_opaque_handle* p_handle) {
	ficapi_opaque_handle& handle = *p_handle;
	handle = static_cast<ficapi_opaque_handle>(malloc(sizeof(ficapi_opaque_handle)));
	handle->value = ficapi_get_dynamic_data();
}

void ficapi_handle_re_create(ficapi_opaque_handle* p_handle) {
	if (*p_handle != nullptr) {
		ficapi_handle_delete(*p_handle);
	}
	ficapi_handle_create(p_handle);
}

void ficapi_handle_delete(ficapi_opaque_handle handle) {
	free(handle);
}

void ficapi_handle_no_alloc_create(ficapi_opaque_handle* p_handle) {
	*p_handle = &opaque_data;
}

void ficapi_handle_no_alloc_re_create(ficapi_opaque_handle* p_handle) {
	if (*p_handle != nullptr) {
		ficapi_handle_no_alloc_delete(*p_handle);
		*p_handle = nullptr;
	}
	*p_handle = &opaque_data;
}

void ficapi_handle_no_alloc_delete(ficapi_opaque_handle handle) {
	// literally nothing
}

int ficapi_handle_get_data(ficapi_opaque_handle_const handle) {
	return handle->value;
}
