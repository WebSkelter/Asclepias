/*
* File:		Graphics.h
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#ifndef ASC_GRAPHICS_H
#define ASC_GRAPHICS_H

#include <vector>
#include <map>
#include <algorithm>

#include "Entity.h"

namespace ASC {
	// Wrapper for OpenGL GLSL vertex/fragment shader program.
	class Shader {
	public:
		/*
		* Assignment operator to copy a shader's ID and camera.
		* @param const Shader& s: The shader to copy.
		*/
		void operator = (const Shader&);
		/*
		* Compile and initialize this shader.
		* @param const std::string& vSource: The vertex shader source code for this shader.
		* @param const std::string& fSource: The fragment shader source code for this shader.
		* @param const std::vector<unsigned int>& attribSizes: The size of each vertex attribute
		* in this shader program in floats.
		* @return Whether the shader could be compiled and initialized.
		*/
		bool init(const std::string&, const std::string&, const std::string&,
			const std::vector<unsigned int>&);
		/*
		* Tell OpenGL to use this shader and enable its vertex attributes.
		*/
		void begin();
		/*
		* Tell OpenGL to stop using this shader and disable its vertex attributes.
		*/
		void end();
		/*
		* Stop this shader and free its memory.
		*/
		void destroy();
		/*
		* @return This shader's camera.
		*/
		inline Camera& getCamera() {
			return camera_;
		}

	private:
		// The OpenGL program ID for this shader.
		GLuint ID_ = 0;
		// Vertex attribute data structure.
		struct VAttrib {
			// The size in bytes of this attribute.
			unsigned int size = 0;
			// The offset to the start of this attribute in vertex data.
			unsigned int offset = 0;
		};
		// The set of vertex attributes for this shader.
		std::vector<VAttrib> attributes_;
		// Size in bytes of all vertex attributes.
		int stride_ = 0;
		// This shader's camera.
		Camera camera_;
		// The name of the mat4 uniform variable for this shader's camera.
		std::string cameraUniform_ = "";
	};

	// App utility for drawing sprites via OpenGL / GLEW.
	class Renderer {
	public:
		// Empty texture returned for errors in getTexture() function.
		static GLuint NO_TEXTURE;
		// Empty font returned for errors in getFont() function.
		static Font NO_FONT;

		/*
		* Initialize the GLEW library and this renderer's memory.
		* @param const glm::vec2& clearColor: The background color to clear the window to.
		* @return Whether GLEW could be initialized.
		*/
		bool init(const glm::vec3&);
		/*
		* Update this renderer's default shader's camera.
		* @param float dt: Timestep since last update.
		*/
		void update(float);
		/*
		* Clear the screen and all previous frame data.
		*/
		void begin();
		/*
		* Submit a single sprite with a custom shader to this renderer.
		* @param T& sprite: Any sprite type with a draw() function and vertices_ member.
		* @param Shader& shader: The shader to draw this sprite with.
		*/
		template <typename T>
		void submit(T& sprite, Shader& shader) {
			sprite.draw();
			Sprite* s = (Sprite*)&sprite;
			if (s->vertices_.empty()) {
				return;
			}
			sprites_.push_back(std::pair<Shader*, Sprite*>(&shader, s));
		}
		/*
		* Submit a single sprite with the default shader.
		* @param T& sprite: Any sprite type with draw() function and vertices_ member.
		*/
		template <typename T>
		void submit(T& sprite) {
			submit(sprite, defaultShader_);
		}
		/*
		* Submit a set of sprites with a custom shader.
		* @param const std::vector<T>& sprites: The set of sprites to submit.
		* @param Shader& shader: The shader to use to draw these sprites.
		*/
		template <typename T>
		void submit(std::vector<T>& sprites, Shader& shader) {
			for (unsigned int i = 0; i < sprites.size(); i++) {
				submit(sprites[i], shader);
			}
		}
		/*
		* Submit a set of sprites with the default shader.
		* @param const std::vector<T>& sprites: The set of sprites to submit.
		*/
		template <typename T>
		void submit(std::vector<T>& sprites) {
			submit(sprites, defaultShader_);
		}
		/*
		* Submit a string to text to draw with a custom shader.
		* @param const std::string& text: The text to draw.
		* @param const glm::vec3& pos: The position to start drawing this text at.
		* @param const glm::vec4& bounds: Rectangle to draw the text inside of (x, y, w, h).
		* @param float scale: The scale to draw this text at.
		* @param const glm::vec3& color: The color to draw this text at.
		* @param const Font& font: The font to draw this text in.
		* @param Alignment hAlign: The horizontal alignment of the text in its bounds.
		* @param Alignment vAlign: The vertical alignment of the text in its bounds.
		* @param Shader& shader: The shader to draw this text with.
		*/
		void submit(const std::string&, const glm::vec3&, const glm::vec4&, float, const glm::vec3&,
			const Font&, Alignment, Alignment, Shader&);
		/*
		* Submmit a string of text to draw with the default shader.
		* @param const std::string& text: The text to draw.
		* @param const glm::vec3& pos: The position to start drawing this text at.
		* @param const glm::vec4& bounds: Rectangle to draw the text inside of (x, y, w, h).
		* @param float scale: The scale to draw this text at.
		* @param const glm::vec3& color: The color to draw this text at.
		* @param const Font& font: The font to draw this text in.
		* @param Alignment hAlign: The horizontal alignment of the text in its bounds.
		* @param Alignment vAlign: The vertical alignment of the text in its bounds.
		*/
		void submit(const std::string&, const glm::vec3&, const glm::vec4&, float, const glm::vec3&,
			const Font&, Alignment, Alignment);
		/*
		* Set this renderer to print the data of the next frame it renderers to the App's log.
		*/
		inline void dump() {
			dump_ = true;
		}
		/*
		* Draw all sprites submitted since the last call to begin().
		*/
		void end();
		/*
		* Get an OpenGL texture ID from memory or load the PNG texture.
		* @param const std::string& fileName: The file path to load the texture from.
		* @return The ID of the texture retrieved / loaded or NO_TEXTURE.
		*/
		const GLuint getTexture(const std::string&);
		/*
		* Get a font from memory or load the TTF font.
		* @param const std::string& fileName: The file path to load the font from.
		* @return The font retrieved / loaded or NO_FONT.
		*/
		const Font& getFont(const std::string&);
		/*
		* Free this renderer's memory.
		*/
		void destroy();
		/*
		* @return This renderer's default shader.
		*/
		inline Shader& getShader() {
			return defaultShader_;
		}
		/*
		* @param const Shader& s: The default shader to use for this renderer.
		*/
		inline void setShader(const Shader& s) {
			defaultShader_ = s;
		}

	private:
		// Whether to print frame data in the next end() call.
		bool dump_ = false;
		// The vertex array object ID for OpenGL.
		GLuint VAO_ = 0;
		// The vertex button object ID for OpenGL.
		GLuint VBO_ = 0;
		// The index buffer object ID for OpenGL.
		GLuint IBO_ = 0;
		// Set of textures loaded from PNG files.
		std::map<std::string, GLuint> textures_;
		// Set of fonts loaded from TTF files.
		std::map<std::string, Font> fonts_;
		// FreeType library instance used to load TTF font files.
		FT_Library freeType_ = 0;
		// Set of text characters submitted this frame.
		std::map<Shader*, std::vector<Glyph>> glyphs_;
		// Set of sprites submitted this frame.
		std::vector<std::pair<Shader*, Sprite*>> sprites_;
		// The default shader to use for this renderer.
		Shader defaultShader_;

		/*
		* Draw a set of vertices and indices with OpenGL and clear them.
		* @param std::vector<float>& vertices: The vertex data to draw.
		* @param std::vector<unsigned int>& indices: The index data to draw.
		*/
		void drawVertices(std::vector<float>&, std::vector<unsigned int>&);
	};
}

#endif