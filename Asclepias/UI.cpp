/*
* File:		UI.cpp
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#include "Asclepias.h"

namespace ASC {
	// Define UIGroup functions.

	void UIGroup::init(Scene& parent, unsigned int ID, Shader& shader, const Font& font) {
		parent_ = &parent;
		ID_ = ID;
		cmpts_.clear();
		nextID_ = 0;
		shader_ = &shader;
		font_ = font;
		enabled_ = true;
		visible_ = true;
		prevWindowDims_ = App::window().getDims();
		ASCLOG(UIGroup, Info, "Initialized UI group ", ID, ".");
	}

	bool UIGroup::addCmpt(UICmpt& cmpt) {
		if (std::find(cmpts_.begin(), cmpts_.end(), &cmpt) != cmpts_.end()) {
			ASCLOG(UIGroup, Warning, "Failed to add UI component to group ", ID_, ".");
			return false;
		}
		cmpt.ID_ = nextID_++;
		cmpt.parent_ = this;
		if (enabled_) {
			App::input().addLstr(cmpt);
		}
		cmpts_.push_back(&cmpt);
		ASCLOG(UIGroup, Info, "Added UI component ", cmpt.getID(), " to group ", ID_, ".");
		return true;
	}

	bool UIGroup::removeCmpt(UICmpt& cmpt) {
		std::vector<UICmpt*>::iterator it = std::find(cmpts_.begin(), cmpts_.end(), &cmpt);
		if (it == cmpts_.end()) {
			ASCLOG(UIGroup, Info, "Failed to remove UI component from group ", ID_, ".");
			return false;
		}
		if (enabled_) {
			App::input().removeLstr(cmpt);
		}
		cmpts_.erase(it);
		ASCLOG(UIGroup, Info, "Removed UI componet ", cmpt.getID(), " from group ", ID_, ".");
		return true;
	}

	void UIGroup::draw() {
		if (visible_) {
			for (UICmpt* cmpt : cmpts_) {
				cmpt->draw();
			}
		}
	}

	void UIGroup::processInput() {
		if (enabled_) {
			for (UICmpt* cmpt : cmpts_) {
				cmpt->processInput();
			}
		}
	}

	void UIGroup::cmptEvent(unsigned int cID, unsigned int eID) {
		parent_->cmptEvent(ID_, cID, eID);
	}

	void UIGroup::update(float dt) {
		if (enabled_) {
			for (UICmpt* cmpt : cmpts_) {
				cmpt->update(dt);
			}
		}
		if (prevWindowDims_ != App::window().getDims()) {
			glm::vec2 wd((float)App::window().getDims().x, (float)App::window().getDims().y);
			glm::vec2 scale(wd.x / (float)prevWindowDims_.x, wd.y / (float)prevWindowDims_.y);
			prevWindowDims_ = wd;
			for (UICmpt* cmpt : cmpts_) {
				glm::vec2 dims = cmpt->getDims();
				dims.x *= scale.x;
				dims.y *= scale.y;
				cmpt->setDims(dims);
				glm::vec3 pos = cmpt->getPos();
				pos.x *= scale.x;
				pos.y *= scale.y;
				cmpt->setPos(pos);
			}
		}
	}

	void UIGroup::destroy() {
		ASCLOG(UIGroup, Info, "Destroying UI group ", ID_, ".");
		for (UICmpt* cmpt : cmpts_) {
			cmpt->destroy();
		}
		cmpts_.clear();
		parent_ = 0;
		ID_ = 0;
		nextID_ = 0;
		shader_ = 0;
		font_ = Renderer::NO_FONT;
		enabled_ = true;
		visible_ = true;
		prevWindowDims_ = glm::ivec2();
	}

	void UIGroup::setEnabled(bool enabled) {
		if (enabled_ == enabled) {
			return;
		}
		enabled_ = enabled;
		if (enabled_) {
			for (UICmpt* cmpt : cmpts_) {
				App::input().addLstr(*cmpt);
			}
			ASCLOG(UIGroup, Info, "Enabled UI group ", ID_, ".");
		}
		else {
			for (UICmpt* cmpt : cmpts_) {
				App::input().removeLstr(*cmpt);
			}
			ASCLOG(UIGroup, Info, "Disabled UI group ", ID_, ".");
		}
	}

	// Define Label functions.

	void Label::init(const glm::vec3& pos, const glm::vec2& dims, const GLuint texture,
		const std::string& text, float textScale, const glm::vec3& textColor, Alignment hAlign,
		Alignment vAlign) {
		bgSprite_.init(glm::vec3(), glm::vec2(), texture);
		setPos(pos);
		setDims(dims);
		text_ = text;
		textScale_ = textScale;
		textColor_ = textColor;
		hAlign_ = hAlign;
		vAlign_ = vAlign;
	}

	void Label::destroy() {
		parent_ = 0;
		ID_ = 0;
		bgSprite_.destroy();
		text_ = "";
		textScale_ = 1.0f;
		textColor_ = glm::vec3();
		hAlign_ = ASC_ALIGN_CENTER;
		vAlign_ = ASC_ALIGN_CENTER;
	}

	glm::vec3 Label::getPos() const {
		glm::ivec2 wd = App::window().getDims();
		return glm::vec3(bgSprite_.pos.x / wd.x, bgSprite_.pos.y / wd.y, bgSprite_.pos.z);
	}

	void Label::setPos(const glm::vec3& pos) {
		glm::ivec2 wd = App::window().getDims();
		bgSprite_.pos = glm::vec3(pos.x * wd.x, pos.y * wd.y, pos.z);
	}

	glm::vec2 Label::getDims() const {
		glm::ivec2 wd = App::window().getDims();
		return glm::vec2(bgSprite_.dims.x / wd.x, bgSprite_.dims.y / wd.y);
	}

	void Label::setDims(const glm::vec2& dims) {
		glm::ivec2 wd = App::window().getDims();
		bgSprite_.dims = glm::vec2(dims.x * wd.x, dims.y * wd.y);
	}

	void Label::draw() {
		App::renderer().submit(bgSprite_, parent_->getShader());
		glm::vec3 textPos(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.pos.z + 0.1f);
		glm::vec4 textBounds(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.dims.x, bgSprite_.dims.y);
		App::renderer().submit(text_, textPos, textBounds, textScale_, textColor_, parent_->getFont(),
			hAlign_, vAlign_, parent_->getShader());
	}

	/*
	* Determine whether a 2D point is inside a rectangle.
	* @param const glm::vec2& p: The point to test (x, y).
	* @param const glm::vec4& r: The rectangle to test (x, y, w, h).
	* @return Whether the point p lies inside the rectangle r.
	*/
	bool InRect(const glm::vec2& p, const glm::vec4& r) {
		if (p.x < r.x || p.x > r.x + r.z) {
			return false;
		}
		if (p.y < r.y || p.y > r.y + r.w) {
			return false;
		}
		return true;
	}

	// Define Button functions.

	glm::vec4 Button::UNSELECTED_COORDS = glm::vec4(0.0f, 0.0f, 1.0f / 3.0f, 1.0f);
	glm::vec4 Button::SELECTED_COORDS = glm::vec4(1.0f / 3.0f, 0.0f, 1.0f / 3.0f, 1.0f);
	glm::vec4 Button::CLICKED_COORDS = glm::vec4(2.0f / 3.0f, 0.0f, 1.0f / 3.0f, 1.0f);

	void Button::init(const glm::vec3& pos, const glm::vec2& dims, const GLuint texture,
		const std::string& labelText, float textScale, const glm::vec3& textColor, Alignment hAlign,
		Alignment vAlign) {
		this->Label::init(pos, dims, texture, labelText, textScale, textColor, hAlign, vAlign);
		bgSprite_.textureRect = UNSELECTED_COORDS;
		selected_ = false;
		clicked_ = false;
	}

	void Button::destroy() {
		this->Label::destroy();
		selected_ = false;
		clicked_ = false;
	}

	void Button::setSelected(bool selected) {
		if (selected) {
			bgSprite_.textureRect = SELECTED_COORDS;
		}
		else {
			bgSprite_.textureRect = UNSELECTED_COORDS;
		}
		selected_ = selected;
	}

	void Button::setClicked(bool clicked) {
		if (clicked) {
			bgSprite_.textureRect = CLICKED_COORDS;
		}
		else {
			if (selected_) {
				bgSprite_.textureRect = SELECTED_COORDS;
			}
			else {
				bgSprite_.textureRect = UNSELECTED_COORDS;
			}
		}
		clicked_ = clicked;
	}

	void Button::processInput() {
		glm::vec2 mp = App::input().getMousePos(parent_->getShader().getCamera());
		if (InRect(mp, glm::vec4(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.dims.x, bgSprite_.dims.y))) {
			if (!selected_) {
				setSelected(true);
				parent_->cmptEvent(ID_, SELECTED_EVENT);
			}
		}
		else {
			if (selected_) {
				setSelected(false);
				parent_->cmptEvent(ID_, UNSELECTED_EVENT);
			}
		}
		if (selected_) {
			if (clicked_) {
				if (App::input().isMouseBtnReleased(ASC_MB_LEFT)) {
					parent_->cmptEvent(ID_, CLICKED_EVENT);
				}
			}
			if (App::input().isMouseBtnDown(ASC_MB_LEFT)) {
				setClicked(true);
			}
			else {
				setClicked(false);
			}
		}
		else {
			setClicked(false);
		}
	}

	// Define Switch functions.

	glm::vec4 Switch::OFF_COORDS = glm::vec4(0.0f, 0.0f, 1.0f / 2.0f, 1.0f);
	glm::vec4 Switch::ON_COORDS = glm::vec4(1.0f / 2.0f, 0.0f, 1.0f / 2.0f, 1.0f);

	void Switch::init(const glm::vec3& pos, const glm::vec2& dims, const GLuint texture,
		const std::string& labelText, float textScale, const glm::vec3& textColor, Alignment lAlign,
		Alignment lHAlign, Alignment lVAlign) {
		this->Label::init(pos, dims, texture, labelText, textScale, textColor, lHAlign, lVAlign);
		bgSprite_.textureRect = OFF_COORDS;
		on_ = false;
		lAlign_ = lAlign;
	}

	void Switch::destroy() {
		this->Label::destroy();
		on_ = false;
		lAlign_ = ASC_ALIGN_TOP;
	}

	void Switch::setOn(bool on) {
		if (on) {
			bgSprite_.textureRect = ON_COORDS;
			parent_->cmptEvent(ID_, ON_EVENT);
		}
		else {
			bgSprite_.textureRect = OFF_COORDS;
			parent_->cmptEvent(ID_, OFF_EVENT);
		}
		on_ = on;
	}

	void Switch::draw() {
		App::renderer().submit(bgSprite_, parent_->getShader());
		glm::vec3 textPos(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.pos.z + 0.1f);
		switch (lAlign_) {
		case ASC_ALIGN_LEFT:
			textPos.x -= bgSprite_.dims.x;
			break;
		case ASC_ALIGN_RIGHT:
			textPos.x += bgSprite_.dims.x;
			break;
		case ASC_ALIGN_BOTTOM:
			textPos.y -= bgSprite_.dims.y;
			break;
		case ASC_ALIGN_TOP:
			textPos.y += bgSprite_.dims.y;
			break;
		}
		glm::vec4 textBounds(textPos.x, textPos.y, bgSprite_.dims.x, bgSprite_.dims.y);
		App::renderer().submit(text_, textPos, textBounds, textScale_, textColor_, parent_->getFont(),
			hAlign_, vAlign_, parent_->getShader());
	}

	void Switch::processInput() {
		glm::vec2 mp = App::input().getMousePos(parent_->getShader().getCamera());
		if (InRect(mp, glm::vec4(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.dims.x, bgSprite_.dims.y))) {
			if (App::input().isMouseBtnPressed(ASC_MB_LEFT)) {
				if (on_) {
					bgSprite_.textureRect = OFF_COORDS;
					on_ = false;
					parent_->cmptEvent(ID_, OFF_EVENT);
				}
				else {
					bgSprite_.textureRect = ON_COORDS;
					on_ = true;
					parent_->cmptEvent(ID_, ON_EVENT);
				}
			}
		}
	}

	void Slider::init(const glm::vec3& pos, const glm::vec2& dims, const GLuint texture,
		const GLuint cursorTexture, float cursorWidth, const std::string& labelText, float textScale,
		const glm::vec3& textColor, Alignment lAlign, Alignment lHAlign, Alignment lVAlign) {
		this->Label::init(pos, dims, texture, labelText, textScale, textColor, lHAlign, lVAlign);
		lAlign_ = lAlign;
		cursor_.init(glm::vec3(), glm::vec2(cursorWidth, 0.0f), cursorTexture);
		value_ = 0.0f;
	}

	void Slider::destroy() {
		this->Label::destroy();
		lAlign_ = ASC_ALIGN_TOP;
		cursor_.destroy();
		value_ = 0.0f;
		selected_ = false;
	}

	void Slider::draw() {
		App::renderer().submit(bgSprite_, parent_->getShader());
		glm::vec3 textPos(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.pos.z + 0.1f);
		switch (lAlign_) {
		case ASC_ALIGN_LEFT:
			textPos.x -= bgSprite_.dims.x;
			break;
		case ASC_ALIGN_RIGHT:
			textPos.x += bgSprite_.dims.x;
			break;
		case ASC_ALIGN_BOTTOM:
			textPos.y -= bgSprite_.dims.y;
			break;
		case ASC_ALIGN_TOP:
			textPos.y += bgSprite_.dims.y;
			break;
		}
		glm::vec4 textBounds(textPos.x, textPos.y, bgSprite_.dims.x, bgSprite_.dims.y);
		App::renderer().submit(text_, textPos, textBounds, textScale_, textColor_, parent_->getFont(),
			hAlign_, vAlign_, parent_->getShader());
		App::renderer().submit(cursor_, parent_->getShader());
	}

	void Slider::processInput() {
		glm::vec2 mp = App::input().getMousePos(parent_->getShader().getCamera());
		if (InRect(mp, glm::vec4(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.dims.x, bgSprite_.dims.y))) {
			if (App::input().isMouseBtnPressed(ASC_MB_LEFT)) {
				selected_ = true;
			}
		}
		if (!App::input().isMouseBtnDown(ASC_MB_LEFT) && selected_) {
			parent_->cmptEvent(ID_, SET_EVENT);
			selected_ = false;
		}
		if (selected_) {
			if (mp.x >= bgSprite_.pos.x + bgSprite_.dims.x - cursor_.dims.x) {
				value_ = 1.0f;
			}
			else if (mp.x < bgSprite_.pos.x) {
				value_ = 0.0f;
			}
			else {
				value_ = (mp.x - bgSprite_.pos.x) / (bgSprite_.dims.x - cursor_.dims.x);
			}
		}
	}

	void Slider::update(float dt) {
		cursor_.pos = glm::vec3(bgSprite_.pos.x + ((bgSprite_.dims.x - cursor_.dims.x) * value_),
			bgSprite_.pos.y, bgSprite_.pos.z + 0.1f);
		cursor_.dims.y = bgSprite_.dims.y;
	}

	// Define Cycle functions.

	void Cycle::init(const glm::vec3& pos, const glm::vec2& dims, const GLuint texture,
		const GLuint buttonTexture, const std::vector<std::string>& values, const std::string& labelText,
		const std::string& prevBText, const std::string& nextBText, float textScale,
		const glm::vec3& textColor, Alignment hAlign, Alignment vAlign, Alignment bAlign,
		Alignment lAlign, Alignment lHAlign, Alignment lVAlign) {
		this->Label::init(pos, dims, texture, (values.empty() ? "" : values.at(0)), textScale,
			textColor, hAlign, vAlign);
		bAlign_ = bAlign;
		values_ = values;
		if (values_.empty()) {
			value_ = -1;
		}
		else {
			value_ = 0;
		}
		labelText_ = labelText;
		lAlign_ = lAlign;
		lHAlign_ = lHAlign;
		lVAlign_ = lVAlign;
		prevB_.init(glm::vec3(), glm::vec2(), buttonTexture, prevBText, textScale, textColor,
			ASC_ALIGN_CENTER, ASC_ALIGN_CENTER);
		prevB_.parent_ = this;
		prevB_.ID_ = 0;
		nextB_.init(glm::vec3(), glm::vec2(), buttonTexture, nextBText, textScale, textColor,
			ASC_ALIGN_CENTER, ASC_ALIGN_CENTER);
		nextB_.parent_ = this;
		nextB_.ID_ = 1;
		setDims(dims);
		setPos(pos);
	}

	void Cycle::destroy() {
		this->Label::destroy();
		bAlign_ = ASC_ALIGN_TOP;
		prevB_.destroy();
		nextB_.destroy();
		values_.clear();
		value_ = -1;
		labelText_ = "";
		lAlign_ = ASC_ALIGN_TOP;
		lHAlign_ = ASC_ALIGN_LEFT;
		lVAlign_ = ASC_ALIGN_CENTER;
	}

	void Cycle::addValue(const std::string& value) {
		if (values_.empty()) {
			values_.push_back(value);
			value_ = 0;
			setText(values_.at(value_));
		}
		else {
			values_.push_back(value);
		}
	}

	void Cycle::removeValue(int index) {
		if (index < 0 || index > values_.size() - 1) {
			return;
		}
		std::vector<std::string> n;
		for (int i = 0; i < values_.size(); i++) {
			if (i != index) {
				n.push_back(values_.at(i));
			}
		}
		values_ = n;
		if (value_ > (int)values_.size() - 1) {
			value_ = (int)values_.size() - 1;
		}
		if (values_.empty()) {
			this->Label::setText("");
			value_ = -1;
		}
		else {
			setText(values_.at(value_));
		}
	}

	int Cycle::getIndexOf(const std::string& value) const {
		int index = -1;
		for (int i = 0; i < values_.size(); i++) {
			if (value == values_.at(i)) {
				index = i;
				break;
			}
		}
		return index;
	}

	const std::string& Cycle::getValue() const {
		if (values_.empty()) {
			return labelText_;
		}
		return values_.at(value_);
	}

	void Cycle::setText(const std::string& text) {
		for (unsigned int i = 0; i < values_.size(); i++) {
			if (text == values_.at(i)) {
				value_ = i;
				this->Label::setText(text);
				return;
			}
		}
		addValue(text);
		setText(text);
	}

	glm::vec3 Cycle::getPos() const {
		switch (bAlign_) {
		case ASC_ALIGN_LEFT:
			return nextB_.getPos();
			break;
		case ASC_ALIGN_RIGHT:
			return this->Label::getPos();
			break;
		}
		return glm::vec3();
	}

	void Cycle::setPos(const glm::vec3& pos) {
		switch (bAlign_) {
		case ASC_ALIGN_LEFT:
			this->Label::setPos(glm::vec3(pos.x + 0.2f * getDims().x, pos.y, pos.z));
			prevB_.setPos(glm::vec3(pos.x, pos.y + nextB_.getDims().y, pos.z));
			nextB_.setPos(pos);
			break;
		case ASC_ALIGN_RIGHT:
			this->Label::setPos(pos);
			prevB_.setPos(glm::vec3(pos.x + 0.8f * getDims().x, pos.y + nextB_.getDims().y, pos.z));
			nextB_.setPos(glm::vec3(pos.x + 0.8f * getDims().x, pos.y, pos.z));
			break;
		}
	}

	glm::vec2 Cycle::getDims() const {
		return glm::vec2(this->Label::getDims().x + prevB_.getDims().x, this->Label::getDims().y);
	}

	void Cycle::setDims(const glm::vec2& dims) {
		this->Label::setDims(glm::vec2(dims.x * 0.8f, dims.y));
		prevB_.setDims(glm::vec2(dims.x * 0.2f, dims.y / 2.0f));
		nextB_.setDims(glm::vec2(dims.x * 0.2f, dims.y / 2.0f));
	}

	void Cycle::cmptEvent(unsigned int cID, unsigned int eID) {
		if (value_ == -1) {
			return;
		}
		if (cID == prevB_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				value_--;
				if (value_ < 0) {
					value_ = (int)values_.size() - 1;
				}
				setText(values_.at(value_));
				parent_->cmptEvent(ID_, SET_EVENT);
			}
		}
		else if (cID == nextB_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				value_++;
				if (value_ > (int)values_.size() - 1) {
					value_ = 0;
				}
				setText(values_.at(value_));
				parent_->cmptEvent(ID_, SET_EVENT);
			}
		}
	}

	void Cycle::draw() {
		glm::vec3 textPos = getPos();
		switch (lAlign_) {
		case ASC_ALIGN_LEFT:
			textPos.x -= getDims().x;
			break;
		case ASC_ALIGN_RIGHT:
			textPos.x += getDims().x;
			break;
		case ASC_ALIGN_BOTTOM:
			textPos.y -= getDims().y;
			break;
		case ASC_ALIGN_TOP:
			textPos.y += getDims().y;
			break;
		}
		glm::vec4 textBounds(textPos.x, textPos.y, getDims().x, getDims().y);
		glm::vec2 wd(App::window().getDims().x, App::window().getDims().y);
		textBounds.x *= wd.x;
		textBounds.y *= wd.y;
		textBounds.z *= wd.x;
		textBounds.w *= wd.y;
		App::renderer().submit(labelText_, textPos, textBounds, textScale_, textColor_, parent_->getFont(),
			lHAlign_, lVAlign_);
		this->Label::draw();
		prevB_.draw();
		nextB_.draw();
	}

	void Cycle::processInput() {
		prevB_.processInput();
		nextB_.processInput();
	}

	void Cycle::update(float dt) {
		prevB_.update(dt);
		nextB_.update(dt);
	}

	// Define TextBox functions.

	glm::vec4 TextBox::UNSELECTED_COORDS = glm::vec4(0.0f, 0.0f, 1.0f / 2.0f, 1.0f);
	glm::vec4 TextBox::SELECTED_COORDS = glm::vec4(1.0f / 2.0f, 0.0f, 1.0f / 2.0f, 1.0f);

	void TextBox::init(const glm::vec3& pos, const glm::vec2& dims, const GLuint texture,
		const GLuint cursorTexture, float cursorWidth, const std::string& labelText,
		const std::string& text, float textScale, const glm::vec3& textColor, int maxChars,
		const std::string& allowedChars, Alignment vAlign, Alignment lAlign, Alignment lHAlign,
		Alignment lVAlign) {
		this->Label::init(pos, dims, texture, text, textScale, textColor, ASC_ALIGN_NONE, vAlign);
		selected_ = false;
		bgSprite_.textureRect = UNSELECTED_COORDS;
		allowedChars_ = allowedChars;
		maxChars_ = maxChars;
		labelText_ = labelText;
		cursor_.init(glm::vec3(), glm::vec2(cursorWidth, 0.0f), cursorTexture);
		cursorPos_ = 0;
		lAlign_ = lAlign;
		lHAlign_ = lHAlign;
		lVAlign_ = lVAlign;
		timer_ = 0.0f;
	}

	void TextBox::setPos(const glm::vec3& pos) {
		this->Label::setPos(pos);
		textPos_ = bgSprite_.pos.x;
	}

	void TextBox::setText(const std::string& text) {
		if (text.length() > maxChars_ && maxChars_ > 0) {
			return;
		}
		textPos_ = bgSprite_.pos.x;
		cursorPos_ = 0;
		text_ = text;
	}

	void TextBox::charTyped(char c) {
		if (!selected_) {
			return;
		}
		if (text_.length() >= maxChars_ && maxChars_ > 0) {
			text_.resize(maxChars_);
			return;
		}
		bool f = false;
		for (char x : allowedChars_) {
			if (x == c) {
				f = true;
				break;
			}
		}
		if (!(allowedChars_.empty() || f)) {
			return;
		}
		std::string nt = "";
		bool s = false;
		for (unsigned int cp = 0; cp < text_.length(); cp++) {
			if (cp == cursorPos_) {
				nt += c;
				s = true;
			}
			nt += text_.at(cp);
		}
		if (s) {
			text_ = nt;
		}
		else {
			text_ += c;
		}
		cursorPos_++;
	}

	void TextBox::draw() {
		App::renderer().submit(bgSprite_, parent_->getShader());
		glm::vec3 labelPos(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.pos.z + 0.1f);
		switch (lAlign_) {
		case ASC_ALIGN_LEFT:
			labelPos.x -= bgSprite_.dims.x;
			break;
		case ASC_ALIGN_RIGHT:
			labelPos.x += bgSprite_.dims.x;
			break;
		case ASC_ALIGN_BOTTOM:
			labelPos.y -= bgSprite_.dims.y;
			break;
		case ASC_ALIGN_TOP:
			labelPos.y += bgSprite_.dims.y;
			break;
		}
		glm::vec4 labelBounds(labelPos.x, labelPos.y, bgSprite_.dims.x, bgSprite_.dims.y);
		App::renderer().submit(labelText_, labelPos, labelBounds, textScale_, textColor_,
			parent_->getFont(), lHAlign_, lVAlign_, parent_->getShader());
		glm::vec3 textPos(textPos_, bgSprite_.pos.y, bgSprite_.pos.z + 0.1f);
		glm::vec4 textBounds(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.dims.x, bgSprite_.dims.y);
		App::renderer().submit(text_, textPos, textBounds, textScale_, textColor_,
			parent_->getFont(), hAlign_, vAlign_, parent_->getShader());
		if (selected_) {
			App::renderer().submit(cursor_, parent_->getShader());
		}
	}

	void TextBox::processInput() {
		glm::vec2 mp = App::input().getMousePos(parent_->getShader().getCamera());
		if (App::input().isMouseBtnDown(ASC_MB_LEFT)) {
			if (InRect(mp, glm::vec4(bgSprite_.pos.x, bgSprite_.pos.y, bgSprite_.dims.x, bgSprite_.dims.y))) {
				if (!selected_) {
					selected_ = true;
					bgSprite_.textureRect = SELECTED_COORDS;
					parent_->cmptEvent(ID_, SELECTED_EVENT);
				}
				float x = textPos_;
				unsigned int cp = 0;
				bool s = false;
				for (unsigned int i = 0; i < text_.length(); i++) {
					float d = parent_->getFont().glyphs.at(text_.at(i)).offset * textScale_;
					if (x + d >= mp.x) {
						s = true;
						cp = i;
						break;
					}
					x += d;
				}
				if (s) {
					cursorPos_ = cp;
				}
				else {
					cursorPos_ = (unsigned int)text_.length();
				}
			}
			else {
				if (selected_ && App::input().isMouseBtnPressed(ASC_MB_LEFT)) {
					selected_ = false;
					bgSprite_.textureRect = UNSELECTED_COORDS;
					parent_->cmptEvent(ID_, UNSELECTED_EVENT);
				}
			}
		}
		if (selected_) {
			if (App::input().isKeyPressed(ASC_CTL_ENTER)) {
				parent_->cmptEvent(ID_, ENTERED_EVENT);
			}
			if (cursorPos_ > 0 && (App::input().isKeyPressed(ASC_CTL_LEFT)
				|| (App::input().isKeyDown(ASC_CTL_LEFT) && timer_ > 10.0f))) {
				cursorPos_--;
				timer_ = 0.0f;
			}
			if (cursorPos_ < text_.length() && (App::input().isKeyPressed(ASC_CTL_RIGHT)
				|| (App::input().isKeyDown(ASC_CTL_RIGHT) && timer_ > 10.0f))) {
				cursorPos_++;
				timer_ = 0.0f;
			}
			if (cursorPos_ > 0 && (App::input().isKeyPressed(ASC_CTL_BACKSPACE)
				|| (App::input().isKeyDown(ASC_CTL_BACKSPACE) && timer_ > 10.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < text_.length(); cp++) {
					if (cursorPos_ != cp + 1) {
						nt += text_.at(cp);
					}
				}
				text_ = nt;
				cursorPos_--;
				timer_ = 0.0f;
			}
			if (cursorPos_ < text_.length() && (App::input().isKeyPressed(ASC_CTL_DELETE)
				|| (App::input().isKeyDown(ASC_CTL_DELETE) && timer_ > 10.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < text_.length(); cp++) {
					if (cursorPos_ != cp) {
						nt += text_.at(cp);
					}
				}
				text_ = nt;
				timer_ = 0.0f;
			}
		}
	}

	void TextBox::update(float dt) {
		if (cursor_.pos.x < bgSprite_.pos.x) {
			textPos_ += bgSprite_.dims.x / 5.0f;
			if (textPos_ > bgSprite_.pos.x) {
				textPos_ = bgSprite_.pos.x;
			}
		}
		else if (cursor_.pos.x + cursor_.dims.x > bgSprite_.pos.x + bgSprite_.dims.x) {
			textPos_ -= bgSprite_.dims.x / 5.0f;
		}
		cursor_.pos = glm::vec3(textPos_, bgSprite_.pos.y, bgSprite_.pos.z + 0.2f);
		cursor_.dims.y = bgSprite_.dims.y;
		for (unsigned int cp = 0; cp < cursorPos_; cp++) {
			cursor_.pos.x += parent_->getFont().glyphs.at(text_.at(cp)).offset * textScale_;
		}
		timer_ += dt;
		if (timer_ > 60.0f) {
			timer_ = 0.0f;
		}
	}
}