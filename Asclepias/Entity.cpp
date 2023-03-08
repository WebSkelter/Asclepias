/*
* File:		Entity.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#include <glm/gtc/matrix_transform.hpp>

#include "Asclepias.h"

namespace ASC {
	// Define Camera functions.

	void Camera::operator = (const Camera& c) {
		pos = c.pos;
		vel = c.vel;
		scale = c.scale;
		prevPos_ = c.prevPos_;
		prevScale_ = c.prevScale_;
		prevWindowDims_ = c.prevWindowDims_;
		updateMatrix();
	}

	void Camera::init() {
		updateMatrix();
	}

	void Camera::update(float dt) {
		pos.x += dt * vel.x;
		pos.y += dt * vel.y;
		scale += dt * scaleVel;
		if (pos != prevPos_ || scale != prevScale_ || prevWindowDims_ != App::window().getDims()) {
			updateMatrix();
			prevPos_ = pos;
			prevScale_ = scale;
			prevWindowDims_ = App::window().getDims();
		}
	}

	void Camera::updateMatrix() {
		glm::vec2 wd((float)App::window().getDims().x, (float)App::window().getDims().y);
		orthoMatrix_ = glm::ortho(0.0f, wd.x, 0.0f, wd.y);
		matrix_ = glm::translate(orthoMatrix_,
			glm::vec3(-pos.x + wd.x / 2.0f, -pos.y + wd.y / 2.0f, 0.0f));
		matrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 0.0f)) * matrix_;
	}

	// Define Sprite functions.

	std::vector<unsigned int> Sprite::Indices = { 0, 1, 2, 2, 3, 0 };

	void Sprite::init(const glm::vec3& pos, const glm::vec2& dims, const GLuint texture) {
		this->pos = pos;
		this->vel = glm::vec2(0.0f, 0.0f);
		this->dims = dims;
		this->texture = texture;
		this->textureRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		this->color = glm::vec3(0.0f, 0.0f, 0.0f);
		vertices_.clear();
	}

	void Sprite::update(float dt) {
		pos.x += dt * vel.x;
		pos.y += dt * vel.y;
		rot += dt * rotVel;
	}

	void Sprite::destroy() {
		pos = glm::vec3();
		vel = glm::vec2();
		rot = 0.0f;
		rotVel = 0.0f;
		dims = glm::vec2();
		texture = 0;
		textureRect = glm::vec4();
		vertices_.clear();
	}

	bool Sprite::intersects(const Sprite& s) {
		if (pos.x > s.pos.x + s.dims.x || pos.x + dims.x < s.pos.x) {
			return false;
		}
		if (pos.y > s.pos.y + s.dims.y || pos.y + dims.y < s.pos.y) {
			return false;
		}
		return true;
	}

	void Sprite::draw() {
		glm::vec3 bl(pos.x, pos.y, pos.z);
		glm::vec3 br(pos.x + dims.x, pos.y, pos.z);
		glm::vec3 tl(pos.x + dims.x, pos.y + dims.y, pos.z);
		glm::vec3 tr(pos.x, pos.y + dims.y, pos.z);
		if ((int)rot % 360 != 0) {
			bl = rotate(bl);
			br = rotate(br);
			tl = rotate(tl);
			tr = rotate(tr);
		}
		if (!reflectH && !reflectV) {
			vertices_ = {
				bl.x, bl.y, bl.z,
				textureRect.x, textureRect.y + textureRect.w,
				color.x, color.y, color.z,
				br.x, br.y, br.z,
				textureRect.x + textureRect.z, textureRect.y + textureRect.w,
				color.x, color.y, color.z,
				tl.x, tl.y, tl.z,
				textureRect.x + textureRect.z, textureRect.y,
				color.x, color.y, color.z,
				tr.x, tr.y, tr.z,
				textureRect.x, textureRect.y,
				color.x, color.y, color.z,
			};
		}
		else if (!reflectH && reflectV) {
			vertices_ = {
				bl.x, bl.y, bl.z,
				textureRect.x + textureRect.z, textureRect.y,
				color.x, color.y, color.z,
				br.x, br.y, br.z,
				textureRect.x, textureRect.y,
				color.x, color.y, color.z,
				tl.x, tl.y, tl.z,
				textureRect.x, textureRect.y + textureRect.w,
				color.x, color.y, color.z,
				tr.x, tr.y, tr.z,
				textureRect.x + textureRect.z, textureRect.y + textureRect.w,
				color.x, color.y, color.z,
			};
		}
		else if (reflectH && !reflectV) {
			vertices_ = {
				bl.x, bl.y, bl.z,
				textureRect.x + textureRect.z, textureRect.y + textureRect.w,
				color.x, color.y, color.z,
				br.x, br.y, br.z,
				textureRect.x, textureRect.y + textureRect.w,
				color.x, color.y, color.z,
				tl.x, tl.y, tl.z,
				textureRect.x, textureRect.y,
				color.x, color.y, color.z,
				tr.x, tr.y, tr.z,
				textureRect.x + textureRect.z, textureRect.y,
				color.x, color.y, color.z,
			};
		}
		else if (reflectH && reflectV) {
			vertices_ = {
				bl.x, bl.y, bl.z,
				textureRect.x, textureRect.y,
				color.x, color.y, color.z,
				br.x, br.y, br.z,
				textureRect.x + textureRect.z, textureRect.y,
				color.x, color.y, color.z,
				tl.x, tl.y, tl.z,
				textureRect.x + textureRect.z, textureRect.y + textureRect.w,
				color.x, color.y, color.z,
				tr.x, tr.y, tr.z,
				textureRect.x, textureRect.y + textureRect.w,
				color.x, color.y, color.z,
			};
		}
	}

#define PI 3.1415926f

	glm::vec3 Sprite::rotate(const glm::vec3& p) {
		float a = (PI / 180.0f) * rot;
		glm::vec2 c = glm::vec2(pos.x, pos.y) + (dims / 2.0f);
		float rx = cos(a) * (p.x - c.x) - sin(a) * (p.y - c.y) + c.x;
		float ry = sin(a) * (p.x - c.x) + cos(a) * (p.y - c.y) + c.y;
		return glm::vec3(rx, ry, p.z);
	}

	// Define Animation functions.

	void Animation::init(const glm::vec3& pos, const glm::vec2& dims, const GLuint texture,
		const glm::ivec2& frameDims, float frameTime) {
		((Sprite*)this)->init(pos, dims, texture);
		frameTime_ = frameTime;
		textureRect = glm::vec4(0.0f, 0.0f, 1.0f / (float)frameDims.x, 1.0f / (float)frameDims.y);
		frameCoords_.resize(frameDims.x * frameDims.y);
		for (int y = 0; y < frameDims.y; y++) {
			for (int x = 0; x < frameDims.x; x++) {
				frameCoords_[x + (y * frameDims.x)] = glm::vec2(textureRect.z * x, textureRect.w * y);
			}
		}
	}

	void Animation::update(float dt) {
		pos.x += dt * vel.x;
		pos.y += dt * vel.y;
		rot += dt * rotVel;
		if (!playing_) {
			return;
		}
		if (timer_ > frameTime_) {
			timer_ = 0.0f;
			frame_ = (frame_ + 1) % frameCoords_.size();
		}
		timer_ += dt;
		textureRect.x = frameCoords_.at(frame_).x;
		textureRect.y = frameCoords_.at(frame_).y;
	}

	void Animation::stop() {
		playing_ = false;
		timer_ = 0.0f;
		frame_ = 0;
		textureRect.x = frameCoords_.at(0).x;
		textureRect.y = frameCoords_.at(0).y;
	}

	void Animation::destroy() {
		pos = glm::vec3();
		vel = glm::vec2();
		rot = 0.0f;
		rotVel = 0.0f;
		dims = glm::vec2();
		texture = 0;
		textureRect = glm::vec4();
		frameTime_ = 0.0f;
		playing_ = false;
		timer_ = 0.0f;
		frame_ = 0;
	}
}