#ifndef MAGNET_RENDER_CBUFFER_DESC_H_
#define MAGNET_RENDER_CBUFFER_DESC_H_

#include "math\matrix4.h"
#include "math\vector4.h"

#define MAX_CASCADE_COUNT 4

namespace magnet {
namespace render {
struct CBufferTransform {
  math::Matrix4f world;
  math::Matrix4f view;
  math::Matrix4f projection;
};

struct CBufferMaterialNormal {
  math::Vector4f ambient;
  math::Vector4f diffuse;
  math::Vector4f specular; // w component is the exponent
  math::Vector4f emissive;
};

struct CBufferShadow {
  math::Matrix4f projection[MAX_CASCADE_COUNT];
  math::Matrix4f view;
  math::Vector4f depth_range;
};

struct CBufferLights {
  math::Vector4f directional_light_dir;
  math::Vector4f directional_light_color;

  math::Vector4f v4PointLightPosition[3];
  math::Vector4f v4PointLightColor[3];
};

struct CBufferToneMapping {
  math::Vector4f params; // (width, height, avg_luminance)
};

#define NUM_SAMPLER_PAIRS 5
struct CBufferSSAO {
  CBufferSSAO() {
    width = 1024.f;
    height = 1024.f;
    far_z = 30.f;
    center_weight = 0.5f;
    inner_radius = 0.3f;
    outer_radius = 0.5f;
    max_radius = 1.0;
    place_holder = 0.f;
  }

  float width;           // depth map width
  float height;          // depth map height
  float far_z;           // z far
  float center_weight;   // center sample weight (percentage in unit sphere volume)

  float inner_radius;    // small radius 
  float outer_radius;    // large radius
  float max_radius;
  float place_holder;

  math::Vector4f sample_points[NUM_SAMPLER_PAIRS];
};

struct CBufferReduceTo1D {
  unsigned int dimension_x;
  unsigned int dimension_y;
  unsigned int texture_x;
  unsigned int texture_y;
};

struct CBufferReduceTo1Pixel {
  unsigned int count;
  unsigned int dimension;
  unsigned int place_holder1;
  unsigned int place_holder2;
};
}  // namespace renderer
}  // namespace magnet
#endif  // MAGNET_RENDER_CBUFFER_DESC_H_
