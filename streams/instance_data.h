/**************************************************************************/
/*  instance_data.h                                                       */
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

#ifndef VOXEL_INSTANCE_DATA_H
#define VOXEL_INSTANCE_DATA_H

#include "../util/span.h"
#include <core/math/transform_3d.h>

// Stores data to pass around until it either gets saved or turned into actual
// instances
struct VoxelInstanceBlockData {
	struct InstanceData {
		Transform3D transform;
	};

	enum VoxelInstanceFormat {
		// Position is lossy-compressed based on the size of the block
		// - uint16_t x;
		// - uint16_t y;
		// - uint16_t z;
		//
		// Scale is uniform and is lossy-compressed to 256 values
		// - uint8_t scale;
		//
		// Rotation is a compressed quaternion with naive quantization of its
		// members to 256 values
		// - uint8_t x;
		// - uint8_t y;
		// - uint8_t z;
		// - uint8_t w;
		FORMAT_SIMPLE_11B_V1 = 0
	};

	static const int POSITION_RESOLUTION = 65536;
	// Because position is quantized we need its range, but it cannot be zero so
	// it may be clamped to this.
	static const float POSITION_RANGE_MINIMUM;

	static const int SIMPLE_11B_V1_SCALE_RESOLUTION = 256;
	static const int SIMPLE_11B_V1_QUAT_RESOLUTION = 256;
	// Because scale is quantized we need its range, but it cannot be zero so it
	// may be clamped to this.
	static const float SIMPLE_11B_V1_SCALE_RANGE_MINIMUM;

	struct LayerData {
		uint16_t id;
		float scale_min;
		float scale_max;
		std::vector<InstanceData> instances;
	};

	float position_range;
	std::vector<LayerData> layers;

	void copy_to(VoxelInstanceBlockData &dst) const {
		// It's all POD so it should work for now
		dst = *this;
	}
};

bool serialize_instance_block_data(const VoxelInstanceBlockData &src,
		std::vector<uint8_t> &dst);
bool deserialize_instance_block_data(VoxelInstanceBlockData &dst,
		Span<const uint8_t> src);

#endif // VOXEL_INSTANCE_DATA_H
