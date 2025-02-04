/**************************************************************************/
/*  dynamic_bitset.h                                                      */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef DYNAMIC_BITSET_H
#define DYNAMIC_BITSET_H

#include <core/error/error_macros.h>
#include <vector>

// STL's bitset is fixed size, and I don't want to depend on Boost
class DynamicBitset {
public:
	inline unsigned int size() const { return _size; }

	inline void resize(unsigned int size) {
		// non-initializing resize
		_bits.resize((size + 63) / 64);
		_size = size;
	}

	void fill(bool v) {
		// Note: padding bits will also be set
		uint64_t m = v ? 0xffffffffffffffff : 0;
		for (auto it = _bits.begin(); it != _bits.end(); ++it) {
			*it = m;
		}
	}

	inline bool get(uint64_t i) const {
#ifdef DEBUG_ENABLED
		CRASH_COND(i >= _size);
#endif
		return _bits[i >> 6] & (uint64_t(1) << (i & uint64_t(63)));
	}

	inline void set(uint64_t i) {
#ifdef DEBUG_ENABLED
		CRASH_COND(i >= _size);
#endif
		_bits[i >> 6] |= uint64_t(1) << (i & uint64_t(63));
	}

	inline void unset(uint64_t i) {
#ifdef DEBUG_ENABLED
		CRASH_COND(i >= _size);
#endif
		_bits[i >> 6] &= ~(uint64_t(1) << (i & uint64_t(63)));
	}

	inline void set(uint64_t i, bool v) {
		if (v) {
			set(i);
		} else {
			unset(i);
		}
	}

private:
	std::vector<uint64_t> _bits;
	unsigned int _size = 0;
};

#endif // DYNAMIC_BITSET_H
