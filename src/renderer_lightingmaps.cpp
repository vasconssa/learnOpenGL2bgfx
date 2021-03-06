#include "shader_manager.h"
#include "renderer_lightingmaps.h"
#include <bgfx/bgfx.h>
#include <bx/math.h>
#include "core/memory/memory.inl"
#include "core/containers/types.h"
#include "core/containers/array.inl"
#include "core/math/types.h"
#include "core/math/color4.inl"
#include "core/math/vector3.inl"
#include "core/math/matrix4x4.inl"
#include "core/math/quaternion.inl"
#include "core/math/constants.h"
#include "core/time.h"
#include "camera.h"
#include <stb_image.h>
#include <stdio.h>

namespace crown {
namespace rendererlightingmap {
	// set up vertex data and indices
    // ------------------------------------------------------------------
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    static float vertices[] = {
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };

	static bgfx::ProgramHandle program_cube;
	static bgfx::ProgramHandle program_lamp;
	static bgfx::VertexBufferHandle vbh_cube;
	static bgfx::VertexBufferHandle vbh_lamp;

	static bgfx::UniformHandle light_position;
	static bgfx::UniformHandle inv_model;
	static bgfx::UniformHandle view_pos;
	static bgfx::UniformHandle ambient;
	static bgfx::UniformHandle diffuse;
    static bgfx::UniformHandle specular;
	static bgfx::UniformHandle specular_shininess;
	static bgfx::UniformHandle light_ambient;
	static bgfx::UniformHandle light_diffuse;
	static bgfx::UniformHandle light_specular;
    static bgfx::TextureHandle diffuse_map;
    static bgfx::TextureHandle specular_map;

	static bgfx::VertexLayout layout_cube;
	static bgfx::VertexLayout layout_light;

	static Vector3 light_pos = vector3(1.2, 1.0, -2.0);
    static Camera cam;

	void init (u16 width, u16 height) 
	{
		f32 aspect = (float)width/(float)height;
		Vector3 pos = vector3(0.0f, 0.0f, -10.0f);
		Quaternion rotation = QUATERNION_IDENTITY;
		Pose pose = {pos, rotation};
		CameraDesc cam_desc = {ProjectionType::PERSPECTIVE, 45.0, 0.1, 100.0};
		cam = camera_create(cam_desc, pose, aspect);

        int w, h, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load("../../../resources/images/container2.png", &w, &h, &nrChannels, 0);
        const bgfx::Memory *im = bgfx::copy(data, nrChannels*w*h);

        int w2, h2, nrChannels2;
        unsigned char *data2 = stbi_load("../../../resources/images/container2_specular.png", &w2, &h2, &nrChannels2, 0);
        const bgfx::Memory *im2 = bgfx::copy(data2, nrChannels2*w2*h2);

        if (data && data2) {
            diffuse_map = bgfx::createTexture2D(w, h, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, im);
            specular_map = bgfx::createTexture2D(w2, h2, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, im2);
        } else {
            printf("Failed to load texture\n");
        }
        stbi_image_free(data);

		ShaderManager sm;
		program_lamp = sm.compile(
				"../../../shaders/colors/v_lamp.sc"
			   ,"../../../shaders/colors/varying_lamp.def.sc"
			   ,"../../../shaders/colors/f_lamp.sc"
		   );
		program_cube = sm.compile(
				 "../../../shaders/lightmaps/v_cube.sc"
				,"../../../shaders/lightmaps/varying_cube.def.sc"
				,"../../../shaders/lightmaps/f_cube.sc"
				);

		layout_cube
			.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
		layout_light
			.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.skip(5 * sizeof(bgfx::AttribType::Float))
			.end();

		vbh_cube = bgfx::createVertexBuffer(
				bgfx::makeRef(vertices, sizeof(vertices)),
				layout_cube
				);
		vbh_lamp = bgfx::createVertexBuffer(
				bgfx::makeRef(vertices, sizeof(vertices)),
				layout_light
				);

		inv_model = bgfx::createUniform("inv_model", bgfx::UniformType::Mat4);
		view_pos = bgfx::createUniform("view_position", bgfx::UniformType::Vec4);
		ambient = bgfx::createUniform("ambient", bgfx::UniformType::Vec4);
		specular_shininess = bgfx::createUniform("specular_shininess", bgfx::UniformType::Vec4);
		light_position = bgfx::createUniform("light_pos", bgfx::UniformType::Vec4);
		light_ambient = bgfx::createUniform("light_ambient", bgfx::UniformType::Vec4);
		light_diffuse = bgfx::createUniform("light_diffuse", bgfx::UniformType::Vec4);
		light_specular= bgfx::createUniform("light_specular", bgfx::UniformType::Vec4);

        diffuse = bgfx::createUniform("diffuse", bgfx::UniformType::Sampler);
        specular = bgfx::createUniform("specular", bgfx::UniformType::Sampler);



		bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
		
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, to_rgba(color4(0.2, 0.3, 0.3, 1.0)), 1.0f, 0);
		bgfx::touch(0);
	}

	void render (u16 width, u16 height, f32 dt)
	{
		static f32 total_time = 0;
		fly_camera_update(cam, dt);

		Matrix4x4 view = get_view(cam);
		Matrix4x4 proj = cam.proj;
		bgfx::setViewTransform(0, to_float_ptr(view), to_float_ptr(proj));

		bgfx::setViewRect(0, 0, 0, width, height);
		bgfx::touch(0);

		//render cube
		Matrix4x4 model;
		set_identity(model);
		bgfx::setTransform(to_float_ptr(model));

		Vector4 obj_color = vector4(1.0, 0.5, 0.31, 1.0);
        Vector4 spec = vector4(0.5, 0.5, 0.5, 32.0);
		Vector4 lgt_pos = vector4(light_pos.x, light_pos.y, light_pos.z, 1.0);
		Vector4 lgt_diffuse = vector4(0.5, 0.5, 0.5, 1.0);
		Vector4 lgt_ambient = vector4(0.2, 0.2, 0.2, 1.0);
		Vector4 lgt_specular = vector4(1.0, 1.0, 1.0, 1.0);
		bgfx::setUniform(light_position, to_float_ptr(lgt_pos));
		bgfx::setUniform(light_ambient, to_float_ptr(lgt_ambient));
		bgfx::setUniform(light_diffuse, to_float_ptr(lgt_diffuse));
		bgfx::setUniform(light_specular, to_float_ptr(lgt_specular));
		bgfx::setUniform(inv_model, to_float_ptr(get_inverted(model)));
		bgfx::setUniform(view_pos, to_float_ptr(cam.pose.position));
		bgfx::setUniform(ambient, to_float_ptr(obj_color));
        bgfx::setUniform(specular_shininess, to_float_ptr(spec));

        bgfx::setTexture(0, diffuse, diffuse_map);
        bgfx::setTexture(1, specular, specular_map);

		bgfx::setVertexBuffer(0, vbh_cube);
		bgfx::setState(BGFX_STATE_DEFAULT ^ BGFX_STATE_CULL_CW);
		bgfx::submit(0, program_cube);

		set_translation(model, light_pos);
		set_scale(model, vector3(0.2, 0.2, 0.2));
		bgfx::setTransform(to_float_ptr(model));
		bgfx::setVertexBuffer(0, vbh_lamp);
		bgfx::setState(BGFX_STATE_DEFAULT ^ BGFX_STATE_CULL_CW);
		bgfx::submit(0, program_lamp);
		total_time += dt;
	}
}
}
