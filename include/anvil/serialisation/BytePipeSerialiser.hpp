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

#ifndef ANVIL_SERIALISATION_BYTE_PIPE_SERIALISER_HPP
#define ANVIL_SERIALISATION_BYTE_PIPE_SERIALISER_HPP

#include <vector>
#include "anvil/serialisation/Serialiser.hpp"
#include "anvil/byte-pipe/BytePipeWriter.hpp"

namespace anvil {
	
	class BytePipeSerialiser final : public Serialiser {
	private:
		struct ArrayHeader {
			Type type;
			uint32_t length;
			Type sub_type;
		};
	
		struct ObjectHeader {
			Type type;
			uint32_t length;
		};
	
		struct State {
			std::string name_buffer;
			std::vector<uint8_t> byte_buffer;
			Type type;
			union {
				struct {
					uint32_t member_count;
				} object_data;
				struct {
					uint32_t length;
					Type type;
				} array_data;
			};
		};

		BytePipe::OutputPipe& _pipe;
		std::vector<State> _states;
	
		void _WriteBytes(const void* data, const size_t bytes);
		void _WriteString(const void* data, const size_t bytes);
		void WriteBytes(const void* data, const size_t bytes, const Type type, const uint32_t count);
	
	public:
		BytePipeSerialiser(BytePipe::OutputPipe&);
		virtual ~BytePipeSerialiser();
	
		// Inherited from Serialiser
	
		void SetNextValueU8(const uint8_t value) final;
		void SetNextValueU16(const uint16_t value) final;
		void SetNextValueU32(const uint32_t value) final;
		void SetNextValueU64(const uint64_t value) final;
		void SetNextValueS8(const int8_t value) final;
		void SetNextValueS16(const int16_t value) final;
		void SetNextValueS32(const int32_t value) final;
		void SetNextValueS64(const int64_t value) final;
		void SetNextValueF32(const float value) final;
		void SetNextValueF64(const double value) final;
		void SetNextValueString(const char* value) final;
		void StartArray() final;
		void EndArray() final;
		void StartObject() final;
		void EndObject() final;
		void SetNextMemberName(const char* name) final;
	
		void SetNextValueU8(const uint8_t* value, const size_t count) final;
		void SetNextValueU16(const uint16_t* value, const size_t count) final;
		void SetNextValueU32(const uint32_t* value, const size_t count) final;
		void SetNextValueU64(const uint64_t* value, const size_t count) final;
		void SetNextValueS8(const int8_t* value, const size_t count) final;
		void SetNextValueS16(const int16_t* value, const size_t count) final;
		void SetNextValueS32(const int32_t* value, const size_t count) final;
		void SetNextValueS64(const int64_t* value, const size_t count) final;
		void SetNextValueF32(const float* value, const size_t count) final;
		void SetNextValueF64(const double* value, const size_t count) final;
	
	};
}

#endif