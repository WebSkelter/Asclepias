/*
* File:		Entity.h
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#ifndef ASC_ENTITY_H
#define ASC_ENTITY_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <ft2build.h>
#include <freetype/freetype.h>

namespace ASC {
	// Abstract 2D object class with position and velocity.
	class Entity {
	public:
		// The position and depth of this entity.
		glm::vec3 pos = glm::vec3();
		// The velocity of this entity.
		glm::vec2 vel = glm::vec2();
	};

	// Class for generating 2D projection matrix with camera position and scale.
	class Camera : public Entity {
	public:
		// The scale of this camera's projection.
		float scale = 1.0f;
		// The velocity of this camera's scale.
		float scaleVel = 0.0f;

		/*
		* Assignment operator for cameras.
		* @param const Camera& cam: Camera to copy.
		*/
		void operator = (const Camera&);
		/*
		* Initialize this camera's memory and projection matrix.
		*/
		void init();
		/*
		* Update this camera's matrix if required.
		* @param float dt: Timestep since the last update.
		*/
		void update(float);
		/*
		* @return The projection matrix for this camera's position and scale.
		*/
		const glm::mat4& getMatrix() const {
			return matrix_;
		}

	private:
		// Basic orthographic projection matrix.
		glm::mat4 orthoMatrix_ = glm::mat4();
		// This camera's current projection matrix.
		glm::mat4 matrix_ = glm::mat4();
		// The position of this camera in the previous update.
		glm::vec3 prevPos_ = glm::vec3();
		// The scale of this camera in the previous update.
		float prevScale_ = 1.0f;
		// The dimensions of the window in the previous update.
		glm::ivec2 prevWindowDims_ = glm::ivec2();

		/*
		* Update this camera's projection matrix based on position and scale.
		*/
		void updateMatrix();
	};

	// Abstract 2D sprite, textured quad.
	class Sprite : public Entity {
	public:
		// The dimensions in pixels of this sprite.
		glm::vec2 dims = glm::vec2();
		// Rotation in degrees of this sprite.
		float rot = 0.0f;
		// Velocity of rotation of this sprite.
		float rotVel = 0.0f;
		// The texture ID for this sprite.
		GLuint texture = 0;
		// The texture coordinates to use for this sprite (x, y, w, h).
		glm::vec4 textureRect = glm::vec4();
		// The background color of this sprite, must be 0 to display texture.
		glm::vec3 color = glm::vec3();
		// Whether to reflect this sprite's texture horizontally.
		bool reflectH = false;
		// Whether to reflect this sprite's texture vertically.
		bool reflectV = false;

		/*
		* Initialize this sprite's memory.
		* @param const glm::vec3& pos: The position and depth of this sprite.
		* @param const glm::vec2& dims: The dimensions of this sprite.
		* @param const GLuint texture: The texture ID for this sprite.
		*/
		void init(const glm::vec3&, const glm::vec2&, const GLuint);
		/*
		* Update this sprite's physics.
		* @param float dt: Timestep since last update.
		*/
		virtual void update(float);
		/*
		* Free this sprite's memory.
		*/
		virtual void destroy();
		/*
		* Test whether this sprite intersects another by rectangular collision detection.
		* @param const Sprite& s: The other sprite to test.
		* @return Whether this sprite intersects the sprite s's rectangle.
		*/
		virtual bool intersects(const Sprite&);

	protected:
		friend class Renderer;

		// Indices for drawing sprites with OpenGL.
		static std::vector<unsigned int> Indices;
		// The vertex data for this sprite.
		std::vector<float> vertices_;

		/*
		* Populate this sprite's vertex data based on position, rotation, color, etc.
		*/
		virtual void draw();
		/*
		* Rotate a 2D point about this sprite's center while preserving depth.
		* @param const glm::vec3& p: The point to rotate with depth.
		* @return The point p rotated by rot about this sprite's center.
		*/
		glm::vec3 rotate(const glm::vec3&);
	};

	// Looping animation class based on 2D sprite.
	class Animation : public Sprite {
	public:
		/*
		* Initialize this animation's memory.
		* @param const glm::vec3& pos: The position for this animation with depth.
		* @param const glm::vec2& dims: The dimensions for this animation.
		* @param const GLuint texture: The texture ID containing all this animation's frames.
		* @param const glm::ivec2& frameDims: The dimensions of this animation's texture *in frames*.
		* @param float frameTime: The timestep to display each frame of this animation for.
		*/
		void init(const glm::vec3&, const glm::vec2&, const GLuint, const glm::ivec2&, float);
		/*
		* Update this animations physics as a sprite and its texture.
		* @param float dt: Timestep since last update.
		*/
		virtual void update(float) override;
		/*
		* Start playing this animation from the current frame.
		*/
		inline virtual void play() {
			playing_ = true;
		}
		/*
		* Pause this animation on the current frame.
		*/
		inline virtual void pause() {
			playing_ = false;
		}
		/*
		* Stop this animation and reset it to its first frame.
		*/
		virtual void stop();
		/*
		* @return Whether this animation is currently playing.
		*/
		inline virtual bool isPlaying() const {
			return playing_;
		}
		/*
		* Free this animation's memory.
		*/
		virtual void destroy() override;

	protected:
		// Timestep to display each frame of this animation for.
		float frameTime_ = 0.0f;
		// The texture coordinates for the frames of this animation in its texture (x, y).
		std::vector<glm::vec2> frameCoords_;
		// Whether this animation is currently playing.
		bool playing_ = false;
		// Timer for tracking animation frame.
		float timer_ = 0.0f;
		// The current frame index of this animation.
		unsigned int frame_ = 0;
	};

	// Enumeration of alignments for submitting text to renderer.
	enum Alignment {
		ASC_ALIGN_LEFT, ASC_ALIGN_RIGHT, ASC_ALIGN_BOTTOM, ASC_ALIGN_TOP, ASC_ALIGN_CENTER,
		ASC_ALIGN_NONE,
	};

	// A single character in a font.
	class Glyph : public Sprite {
	public:
		// Bearing for this character's texture.
		glm::ivec2 bearing = glm::ivec2();
		// Width of this character.
		unsigned int offset = 0;
	};

	// Set of glyphs loaded from TTF file via FreeType.
	class Font {
	public:
		// The minimum vertical character bearing of this font.
		float minBearing = 0.0f;
		// The maximum vertical character bearing of this font.
		float maxBearing = 0.0f;
		// Set of characters in this font.
		std::map<char, Glyph> glyphs;
	};
}

#endif