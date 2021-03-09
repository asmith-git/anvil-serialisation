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

#ifndef ANVIL_SERIALISATION_SERIALISER_HPP
#define ANVIL_SERIALISATION_SERIALISER_HPP

#include <cstdint>

namespace anvil {
	class Serialiser {
	public:
		enum Type : uint8_t {
			TYPE_UNSIGNED_8,
			TYPE_UNSIGNED_16,
			TYPE_UNSIGNED_32,
			TYPE_UNSIGNED_64,
			TYPE_SIGNED_8,
			TYPE_SIGNED_16,
			TYPE_SIGNED_32,
			TYPE_SIGNED_64,
			TYPE_FLOAT_32,
			TYPE_FLOAT_64,
			TYPE_STRING,
			TYPE_ARRAY,
			TYPE_OBJECT
		};

		Serialiser() {}
		virtual ~Serialiser() {}

		virtual void SetNextValueU8(const uint8_t value) = 0;
		virtual void SetNextValueU16(const uint16_t value) = 0;
		virtual void SetNextValueU32(const uint32_t value) = 0;
		virtual void SetNextValueU64(const uint64_t value) = 0;
		virtual void SetNextValueS8(const int8_t value) = 0;
		virtual void SetNextValueS16(const int16_t value) = 0;
		virtual void SetNextValueS32(const int32_t value) = 0;
		virtual void SetNextValueS64(const int64_t value) = 0;
		virtual void SetNextValueF32(const float value) = 0;
		virtual void SetNextValueF64(const double value) = 0;
		virtual void SetNextValueString(const char* value) = 0;

		virtual void StartArray() = 0;
		virtual void EndArray() = 0;

		virtual void StartObject() = 0;
		virtual void EndObject() = 0;

		virtual void SetNextMemberName(const char* name) = 0;
	};
}

#endif