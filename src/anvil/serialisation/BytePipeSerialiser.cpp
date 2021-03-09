// MIT License
// 
// Copyright(c) 2021 Adam Smith
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "anvil/serialisation/BytePipeSerialiser.hpp"

namespace anvil {
	
	// BytePipeSerialiser

	void BytePipeSerialiser::_WriteBytes(const void* data, const size_t bytes) {
		State& state = _states.back();

		if (_states.empty()) {
			// Write to stream directly
			_pipe.WriteBytes(data, bytes);
		} else {
			// Write to state buffer
			state.byte_buffer.reserve(state.byte_buffer.size() + bytes);
			const uint8_t* const byte_ptr = static_cast<const uint8_t*>(data);
			for (uint32_t i = 0; i < bytes; ++i) {
				state.byte_buffer.push_back(byte_ptr[i]);
			}
		}
	}

	void BytePipeSerialiser::_WriteString(const void* data, const size_t bytes) {
		// Write string length first
		uint32_t length = static_cast<uint32_t>(bytes);
		_WriteBytes(&length, sizeof(uint32_t));

		// Then write string contents
		_WriteBytes(data, bytes);
	}

	void BytePipeSerialiser::WriteBytes(const void* data, const size_t bytes, const Type type, const uint32_t count) {
		State& state = _states.back();
		if (state.type == TYPE_ARRAY) {
			if (state.array_data.length == 0) {
				state.array_data.type = type;
			} else if (state.array_data.type != type) {
				throw std::runtime_error("BinarySerialiser::WriteBytes : Type of value does not match previous values in array");
			}
			state.array_data.length += count;
		} else if(state.type == TYPE_OBJECT) {
			if (count != 1) throw std::runtime_error("BinarySerialiser::WriteBytes : Undefined member names");
			if (state.name_buffer.empty()) throw std::runtime_error("BinarySerialiser::WriteBytes : Undefined member name");
			++state.object_data.member_count;

			// Write the object name
			_WriteString(state.name_buffer.c_str(), state.name_buffer.size());
			state.name_buffer.clear();
		} else {
			if (count != 1) throw std::runtime_error("BinarySerialiser::WriteBytes : Current value is not an array or object");
		}

		if (type == TYPE_STRING) {
			if (count != 1) throw std::runtime_error("BinarySerialiser::WriteBytes : Can only write one string at a time");
			_WriteString(data, bytes);
		} else {
			_WriteBytes(data, bytes);
		}

	}

	BytePipeSerialiser::BytePipeSerialiser(BytePipe::OutputPipe& pipe) :
		_pipe(pipe)
	{}

	BytePipeSerialiser::~BytePipeSerialiser() {
		_pipe.Flush();
	}

	void BytePipeSerialiser::SetNextValueU8(const uint8_t value) {
		WriteBytes(&value, sizeof(value), TYPE_UNSIGNED_8, 1u);
	}

	void BytePipeSerialiser::SetNextValueU16(const uint16_t value) {
		WriteBytes(&value, sizeof(value), TYPE_UNSIGNED_16, 1u);
	}

	void BytePipeSerialiser::SetNextValueU32(const uint32_t value) {
		WriteBytes(&value, sizeof(value), TYPE_UNSIGNED_32, 1u);
	}

	void BytePipeSerialiser::SetNextValueU64(const uint64_t value) {
		WriteBytes(&value, sizeof(value), TYPE_UNSIGNED_64, 1u);
	}

	void BytePipeSerialiser::SetNextValueS8(const int8_t value) {
		WriteBytes(&value, sizeof(value), TYPE_SIGNED_8, 1u);
	}

	void BytePipeSerialiser::SetNextValueS16(const int16_t value) {
		WriteBytes(&value, sizeof(value), TYPE_SIGNED_16, 1u);
	}

	void BytePipeSerialiser::SetNextValueS32(const int32_t value) {
		WriteBytes(&value, sizeof(value), TYPE_SIGNED_32, 1u);
	}

	void BytePipeSerialiser::SetNextValueS64(const int64_t value) {
		WriteBytes(&value, sizeof(value), TYPE_SIGNED_64, 1u);
	}

	void BytePipeSerialiser::SetNextValueF32(const float value) {
		WriteBytes(&value, sizeof(value), TYPE_FLOAT_32, 1u);
	}

	void BytePipeSerialiser::SetNextValueF64(const double value) {
		WriteBytes(&value, sizeof(value), TYPE_FLOAT_64, 1u);
	}

	void BytePipeSerialiser::SetNextValueString(const char* value) {
		WriteBytes(&value, strlen(value), TYPE_STRING, 1u);
	}

	void BytePipeSerialiser::StartArray() {
		// Create a new state
		_states.push_back(State());
		State& state = _states.back();

		// Initialise state
		state.type = TYPE_ARRAY;
		state.array_data.length = 0u;
		state.array_data.type = TYPE_UNSIGNED_8;

		// Write header
		ArrayHeader header;
		header.type = TYPE_ARRAY;
		state.array_data.length = 0u;
		header.sub_type = TYPE_UNSIGNED_8;
		state.byte_buffer.resize(sizeof(header));
		memcpy(state.byte_buffer.data(), &header, sizeof(header));
	}

	void BytePipeSerialiser::EndArray() {
		// Check value is an array
		if (_states.back().type != TYPE_ARRAY) throw std::runtime_error("BinarySerialiser::EndArray : Current value is not an array");

		// Remove the state from the stack
		State state = std::move(_states.back());
		_states.pop_back();

		// Update header
		ArrayHeader& header = *reinterpret_cast<ArrayHeader*>(state.byte_buffer.data());
		header.length = state.array_data.length;
		header.sub_type = state.array_data.type;

		// Write array
		WriteBytes(state.byte_buffer.data(), state.byte_buffer.size(), TYPE_ARRAY, 1);
	}

	void BytePipeSerialiser::StartObject() {
		// Create a new state
		_states.push_back(State());
		State& state = _states.back();

		// Initialise state
		state.type = TYPE_OBJECT;
		state.object_data.member_count = 0u;

		// Write header
		ObjectHeader header;
		header.type = TYPE_OBJECT;
		state.array_data.length = 0u;
		state.byte_buffer.resize(sizeof(header));
		memcpy(state.byte_buffer.data(), &header, sizeof(header));
	}

	void BytePipeSerialiser::EndObject() {
		// Check value is an object
		if (_states.back().type != TYPE_OBJECT) throw std::runtime_error("BinarySerialiser::EndObject : Current value is not an object");

		// Remove the state from the stack
		State state = std::move(_states.back());
		_states.pop_back();

		// Update header
		ObjectHeader& header = *reinterpret_cast<ObjectHeader*>(state.byte_buffer.data());
		header.length = state.array_data.length;

		// Write array
		WriteBytes(state.byte_buffer.data(), state.byte_buffer.size(), TYPE_OBJECT, 1);
	}

	void BytePipeSerialiser::SetNextMemberName(const char* name) {
		State& state = _states.back();
		if (state.type != TYPE_OBJECT) throw std::runtime_error("BinarySerialiser::SetNextMemberName : Current value is not an object");
		state.name_buffer = name;
	}

	void BytePipeSerialiser::SetNextValueU8(const uint8_t* value, const size_t count) {
		WriteBytes(&value, sizeof(uint8_t) * count, TYPE_UNSIGNED_8, count);
	}

	void BytePipeSerialiser::SetNextValueU16(const uint16_t* value, const size_t count) {
		WriteBytes(&value, sizeof(uint16_t) * count, TYPE_UNSIGNED_16, count);
	}

	void BytePipeSerialiser::SetNextValueU32(const uint32_t* value, const size_t count) {
		WriteBytes(&value, sizeof(uint32_t) * count, TYPE_UNSIGNED_32, count);
	}

	void BytePipeSerialiser::SetNextValueU64(const uint64_t* value, const size_t count) {
		WriteBytes(&value, sizeof(uint64_t) * count, TYPE_UNSIGNED_64, count);
	}

	void BytePipeSerialiser::SetNextValueS8(const int8_t* value, const size_t count) {
		WriteBytes(&value, sizeof(int8_t) * count, TYPE_SIGNED_8, count);
	}

	void BytePipeSerialiser::SetNextValueS16(const int16_t* value, const size_t count) {
		WriteBytes(&value, sizeof(int16_t) * count, TYPE_SIGNED_16, count);
	}

	void BytePipeSerialiser::SetNextValueS32(const int32_t* value, const size_t count) {
		WriteBytes(&value, sizeof(int32_t) * count, TYPE_SIGNED_32, count);
	}

	void BytePipeSerialiser::SetNextValueS64(const int64_t* value, const size_t count) {
		WriteBytes(&value, sizeof(int64_t) * count, TYPE_SIGNED_64, count);
	}

	void BytePipeSerialiser::SetNextValueF32(const float* value, const size_t count) {
		WriteBytes(&value, sizeof(float) * count, TYPE_FLOAT_32, count);
	}

	void BytePipeSerialiser::SetNextValueF64(const double* value, const size_t count) {
		WriteBytes(&value, sizeof(double) * count, TYPE_FLOAT_64, count);
	}
}