/*
* File:		Graphics.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#include <fstream>

#include "Asclepias.h"

// Reference to picoPNG's decodePNG function.

extern int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width,
	unsigned long& image_height, const unsigned char* in_png, size_t in_size,
	bool convert_to_rgba32 = true);

namespace ASC {
	// Define Shader functions.

	void Shader::operator = (const Shader& s) {
		ID_ = s.ID_;
		attributes_.clear();
		for (const VAttrib& v : s.attributes_) {
			attributes_.push_back(v);
		}
		stride_ = s.stride_;
		camera_ = s.camera_;
		cameraUniform_ = s.cameraUniform_;
	}

	bool Shader::init(const std::string& vSource, const std::string& fSource,
		const std::string& cameraUniform, const std::vector<unsigned int>& attribSizes) {
		camera_.init();
		cameraUniform_ = cameraUniform;
		const char* vSourceC = vSource.c_str();
		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vShader, 1, &vSourceC, 0);
		glCompileShader(vShader);
		int vs;
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &vs);
		if (!vs) {
			char vbuf[512];
			glGetShaderInfoLog(vShader, 512, 0, vbuf);
			ASCLOG(Shader, Warning, "Failed to compile vertex shader source.\n", vbuf);
			return false;
		}
		ASCLOG(Shader, Info, "Compiled vertex shader ", vShader, ".");
		const char* fSourceC = fSource.c_str();
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShader, 1, &fSourceC, 0);
		glCompileShader(fShader);
		int fs;
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &fs);
		if (!fs) {
			char fbuf[512];
			glGetShaderInfoLog(fShader, 512, 0, fbuf);
			ASCLOG(Shader, Warning, "Failed to compile fragment shader source.\n", fbuf);
			return false;
		}
		ASCLOG(Shader, Info, "Compiled fragment shader ", fShader, ".");
		ID_ = glCreateProgram();
		glAttachShader(ID_, vShader);
		glAttachShader(ID_, fShader);
		glLinkProgram(ID_);
		int ps;
		glGetProgramiv(ID_, GL_LINK_STATUS, &ps);
		if (!ps) {
			char pbuf[512];
			glGetProgramInfoLog(ID_, 512, 0, pbuf);
			ASCLOG(Shader, Warning, "Failed to link shader program.\n", pbuf);
			return false;
		}
		glDeleteShader(vShader);
		glDeleteShader(fShader);
		attributes_.resize(attribSizes.size());
		stride_ = 0;
		for (unsigned int a = 0; a < attribSizes.size(); a++) {
			attributes_[a].size = attribSizes.at(a);
			attributes_[a].offset = stride_ * sizeof(float);
			stride_ += attribSizes.at(a);
		}
		stride_ *= sizeof(float);
		ASCLOG(Shader, Info, "Linked shader program ", ID_, ".");
		return true;
	}

	void Shader::begin() {
		glUseProgram(ID_);
		for (unsigned int a = 0; a < attributes_.size(); a++) {
			glVertexAttribPointer(a, attributes_.at(a).size, GL_FLOAT, GL_FALSE, stride_,
				(void*)attributes_.at(a).offset);
			glEnableVertexAttribArray(a);
		}
		if (!cameraUniform_.empty()) {
			GLint loc = glGetUniformLocation(ID_, cameraUniform_.c_str());
			if (loc == -1) {
				return;
			}
			glUniformMatrix4fv(loc, 1, GL_FALSE, &(camera_.getMatrix()[0][0]));
		}
	}

	void Shader::end() {
		for (unsigned int a = 0; a < attributes_.size(); a++) {
			glDisableVertexAttribArray(a);
		}
		glUseProgram(0);
	}
	
	void Shader::destroy() {
		ASCLOG(Shader, Info, "Destroying shader ", ID_, ".");
		glDeleteProgram(ID_);
		ID_ = 0;
		attributes_.clear();
		stride_ = 0;
		cameraUniform_ = "";
	}

	// Define Renderer functions.

	GLuint Renderer::NO_TEXTURE = 0;
	Font Renderer::NO_FONT;

	bool Renderer::init(const glm::vec3& clearColor) {
		if (glewInit() != GLEW_OK) {
			ASCLOG(Renderer, Error, "Failed to initialize GLEW library.");
			return false;
		}
		const GLubyte* version = glGetString(GL_VERSION);
		ASCLOG(Renderer, Info, "Initialized GLEW library: ", version, ".");
		if (FT_Init_FreeType(&freeType_) != FT_Err_Ok) {
			ASCLOG(Renderer, Info, "Failed to initialize FreeType library.");
			return false;
		}
		ASCLOG(Renderer, Info, "Initialized FreeType library.");
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPixelStorei(GL_UNPACK_ALIGNMENT, GL_TRUE);
		glGenVertexArrays(1, &VAO_);
		glBindVertexArray(VAO_);
		glGenBuffers(1, &VBO_);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_);
		glGenBuffers(1, &IBO_);
		glBindBuffer(GL_ARRAY_BUFFER, IBO_);
		defaultShader_.init(
			"#version 330 core\n"
			"layout (location = 0) in vec3 vPos;\n"
			"layout (location = 1) in vec2 vTexPos;\n"
			"layout (location = 2) in vec3 vColor;\n"
			"uniform mat4 cameraMatrix;\n"
			"out vec2 fTexPos;\n"
			"out vec3 fColor;\n"
			"void main() {\n"
			"	gl_Position = cameraMatrix * vec4(vPos, 1.0);\n"
			"	fTexPos = vTexPos;\n"
			"	fColor = vColor;\n"
			"}\n",

			"#version 330 core\n"
			"in vec2 fTexPos;\n"
			"in vec3 fColor;\n"
			"out vec4 FragColor;\n"
			"uniform sampler2D texSampler;\n"
			"void main() {\n"
			"	if (fColor.x == 0.0 && fColor.y == 0.0 && fColor.z == 0.0) {\n"
			"		FragColor = texture(texSampler, fTexPos);\n"
			"	} else {\n"
			"		vec4 texColor = vec4(1.0, 1.0, 1.0, texture(texSampler, fTexPos).r);\n"
			"		FragColor = vec4(fColor, 1.0) * texColor;\n"
			"	}\n"
			"}\n",

			"cameraMatrix", { 3, 2, 3 }
		);
		ASCLOG(Renderer, Info, "Compiled default shader program.");
		return true;
	}

	void Renderer::update(float dt) {
		defaultShader_.getCamera().update(dt);
	}

	void Renderer::begin() {
		glClear(GL_COLOR_BUFFER_BIT);
		sprites_.clear();
		glyphs_.clear();
	}

	void Renderer::submit(const std::string& text, const glm::vec3& pos, const glm::vec4& bounds,
		float scale, const glm::vec3& color, const Font& font, Alignment hAlign, Alignment vAlign,
		Shader& shader) {
		if (text.empty()) {
			return;
		}
		if (font.glyphs.empty()) {
			return;
		}
		if (glyphs_.find(&shader) == glyphs_.end()) {
			glyphs_[&shader] = std::vector<Glyph>(0);
		}
		float labelWidth = 0.0f;
		float minBearing = font.minBearing * scale;
		float maxBearing = font.maxBearing * scale;
		for (char c : text) {
			labelWidth += font.glyphs.at(c).offset * scale;
		}
		float labelHeight = minBearing + maxBearing;
		float x = 0.0f;
		float y = 0.0f;
		if (hAlign == ASC_ALIGN_LEFT) {
			x = bounds.x;
		}
		else if (hAlign == ASC_ALIGN_RIGHT) {
			x = (bounds.x + bounds.z) - labelWidth;
		}
		else if (hAlign == ASC_ALIGN_CENTER) {
			float m = bounds.x + (bounds.z / 2.0f);
			x = m - (labelWidth / 2.0f);
		}
		else {
			x = pos.x;
		}
		if (vAlign == ASC_ALIGN_BOTTOM) {
			y = bounds.y + minBearing;
		}
		else if (vAlign == ASC_ALIGN_TOP) {
			y = (bounds.y + bounds.w) - labelHeight;
		}
		else if (vAlign == ASC_ALIGN_CENTER) {
			float m = bounds.y + (bounds.w / 2.0f);
			y = m - (labelHeight / 2.0f);
		}
		else {
			y = pos.y;
		}
		x -= font.glyphs.at(text.at(0)).bearing.x * scale;
		for (char c : text) {
			Glyph g = font.glyphs.at(c);
			g.init(glm::vec3(x + g.bearing.x * scale, y - ((g.dims.y - g.bearing.y) * scale), pos.z),
				g.dims * scale, g.texture);
			if (!(g.pos.x < bounds.x
				|| g.pos.x + g.dims.x > bounds.x + bounds.z
				|| g.pos.y < bounds.y
				|| g.pos.y + g.dims.y > bounds.y + bounds.w)) {
				g.color = color;
				g.draw();
				glyphs_[&shader].push_back(g);
			}
			x += g.offset * scale;
		}
	}

	void Renderer::submit(const std::string& text, const glm::vec3& pos, const glm::vec4& bounds,
		float scale, const glm::vec3& color, const Font& font, Alignment hAlign, Alignment vAlign) {
		submit(text, pos, bounds, scale, color, font, hAlign, vAlign, defaultShader_);
	}

	/*
	* Compare two sprite structures by their depth.
	* @param const std::pair<Shader*, Sprite*>& a: The first sprite to test.
	* @param const std::pair<Shader*, Sprite*>& b: The second sprite to test.
	* @return Whether sprite a's depth is less then sprite b's.
	*/
	bool cmp_sprite_depth(const std::pair<Shader*, Sprite*>& a, const std::pair<Shader*, Sprite*>& b) {
		if (a.second == 0 || b.second == 0) {
			return false;
		}
		return a.second->pos.z < b.second->pos.z;
	}

	void Renderer::end() {
		if (dump_) {
			ASCLOG(Renderer, Info, "Starting draw.");
		}
		if (!glyphs_.empty()) {
			if (dump_) {
				ASCLOG(Renderer, Info, "Submitting ", glyphs_.size(), " text glyphs.");
			}
			for (std::pair<Shader*, std::vector<Glyph>> p : glyphs_) {
				submit(glyphs_[p.first], *p.first);
			}
		}
		if (sprites_.empty()) {
			if (dump_) {
				ASCLOG(Renderer, Info, "No sprites submitted.");
			}
			return;
		}
		std::stable_sort(sprites_.begin(), sprites_.end(), cmp_sprite_depth);
		if (dump_) {
			ASCLOG(Renderer, Info, "Sorted sprites by depth.");
		}
		glBindVertexArray(VAO_);
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		unsigned int count = 0;
		if (dump_) {
			ASCLOG(Renderer, Info, "Iterating ", sprites_.size(), " sprites.");
		}
		Shader* cShader = sprites_.front().first;
		cShader->begin();
		GLuint cTexture = sprites_.front().second->texture;
		glBindTexture(GL_TEXTURE_2D, cTexture);
		if (dump_) {
			ASCLOG(Renderer, Info, "Bound initial shader ", cShader, " and texture ", cTexture, ".");
		}
		for (const std::pair<Shader*, Sprite*>& sPair : sprites_) {
			if (cShader != sPair.first) {
				if (dump_) {
					ASCLOG(Renderer, Info, "New shader found ", sPair.first, ".");
				}
				if (count > 0) {
					if (dump_) {
						ASCLOG(Renderer, Info, "Rendering ", count, " sprites from last shader.");
					}
					drawVertices(vertices, indices);
					count = 0;
				}
				cShader->end();
				cShader = sPair.first;
				cShader->begin();
			}
			if (cTexture != sPair.second->texture) {
				if (dump_) {
					ASCLOG(Renderer, Info, "New texture found ", sPair.second->texture, ".");
				}
				if (count > 0) {
					if (dump_) {
						ASCLOG(Renderer, Info, "Rendering ", count, " sprites from last texture.");
					}
					drawVertices(vertices, indices);
					count = 0;
				}
				cTexture = sPair.second->texture;
				glBindTexture(GL_TEXTURE_2D, cTexture);
			}
			for (float v : sPair.second->vertices_) {
				vertices.push_back(v);
			}
			for (unsigned int i : Sprite::Indices) {
				indices.push_back(i + 4 * count);
			}
			count++;
		}
		if (count > 0) {
			if (dump_) {
				ASCLOG(Renderer, Info, "Drawing ", count, "left over sprites.");
			}
			drawVertices(vertices, indices);
		}
		if (dump_) {
			dump_ = false;
		}
	}

	const GLuint Renderer::getTexture(const std::string& fileName) {
		if (textures_.find(fileName) != textures_.end()) {
			return textures_.at(fileName);
		}
		ASCLOG(Renderer, Info, "Retrieving texture from ", fileName, ".");
		std::ifstream texFile(fileName, std::ios::in | std::ios::binary | std::ios::ate);
		if (!texFile.is_open()) {
			ASCLOG(Renderer, Warning, "Failed to open texture file ", fileName, ".");
			return NO_TEXTURE;
		}
		std::streamsize fSize = 0;
		if (!texFile.seekg(0, std::ios::end).good()) {
			ASCLOG(Renderer, Warning, "Failed to read size of texture file ", fileName, ".");
			return NO_TEXTURE;
		}
		fSize = texFile.tellg();
		if (!texFile.seekg(0, std::ios::beg).good()) {
			ASCLOG(Renderer, Warning, "Failed to read size of texture file ", fileName, ".");
			return NO_TEXTURE;
		}
		fSize -= texFile.tellg();
		if (fSize <= 0) {
			ASCLOG(Renderer, Warning, "Texture file ", fileName, " has invalid size.");
			return NO_TEXTURE;
		}
		std::vector<unsigned char> buffer;
		buffer.resize((size_t)fSize);
		texFile.read((char*)(&buffer[0]), fSize);
		texFile.close();
		std::vector<unsigned char> texData;
		unsigned long w = 0, h = 0;
		if (decodePNG(texData, w, h, &buffer[0], (size_t)fSize) != 0) {
			ASCLOG(Renderer, Warning, "Failed to decode PNG from ", fileName, ".");
			return NO_TEXTURE;
		}
		GLuint texID = 0;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texData[0]);
		glBindTexture(GL_TEXTURE_2D, 0);
		textures_[fileName] = texID;
		return texID;
	}

	const Font& Renderer::getFont(const std::string& fileName) {
		if (fonts_.find(fileName) != fonts_.end()) {
			return fonts_.at(fileName);
		}
		ASCLOG(Renderer, Info, "Retrieving font ", fileName, ".");
		FT_Face face;
		if (FT_New_Face(freeType_, fileName.c_str(), 0, &face) != FT_Err_Ok) {
			ASCLOG(Renderer, Warning, "Failed to load font file ", fileName, ".");
			return NO_FONT;
		}
		FT_Set_Pixel_Sizes(face, 0, 48);
		Font font;
		for (unsigned char g = 0; g < 128; g++) {
			if (FT_Load_Char(face, g, FT_LOAD_RENDER) != FT_Err_Ok) {
				ASCLOG(Renderer, Warning, "Failed to load character ", g, " from font ", fileName, ".");
				continue;
			}
			GLuint texID;
			glGenTextures(1, &texID);
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
				0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
			font.glyphs[g] = Glyph();
			font.glyphs[g].dims = glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
			font.glyphs[g].bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
			font.glyphs[g].offset = face->glyph->advance.x >> 6;
			font.glyphs[g].texture = texID;
			float mib = font.glyphs[g].dims.y - (float)font.glyphs[g].bearing.y;
			if (mib > font.minBearing) {
				font.minBearing = mib;
			}
			float mab = (float)font.glyphs[g].bearing.y;
			if (mab > font.maxBearing) {
				font.maxBearing = mab;
			}
		}
		FT_Done_Face(face);
		fonts_[fileName] = font;
		return fonts_.at(fileName);
	}

	void Renderer::destroy() {
		ASCLOG(Renderer, Info, "Destroying renderer.");
		dump_ = false;
		sprites_.clear();
		unsigned int tCount = 0;
		for (std::pair<const std::string, GLuint>& t : textures_) {
			glDeleteTextures(1, &t.second);
			tCount++;
		}
		ASCLOG(Renderer, Info, "Freed ", tCount, " textures from OpenGL.");
		textures_.clear();
		unsigned int fCount = 0;
		for (std::pair<const std::string, Font>& f : fonts_) {
			for (std::pair<const char, Glyph>& g : f.second.glyphs) {
				glDeleteTextures(1, &g.second.texture);
				g.second.destroy();
			}
			fCount++;
		}
		ASCLOG(Renderer, Info, "Freed ", fCount, " fonts from OpenGL.");
		fonts_.clear();
		FT_Done_FreeType(freeType_);
		freeType_ = 0;
		defaultShader_.destroy();
		glUseProgram(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &IBO_);
		IBO_ = 0;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &VBO_);
		VBO_ = 0;
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAO_);
		VAO_ = 0;
	}

	void Renderer::drawVertices(std::vector<float>& vertices, std::vector<unsigned int>& indices) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(),
			GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);
		vertices.clear();
		indices.clear();
	}
}