/*
* File:		UI.h
* Author:	WebSkelter (webskelter@gmail.com)
* Created:	2023.03.07
*/

#ifndef ASC_UI_H
#define ASC_UI_H

#include "Entity.h"

namespace ASC {
	// Forward declare Scene, Shader, and UIGroup classes.
	class Scene;
	class Shader;
	class UIGroup;

	// Abstract interface for any controller of multiple UI components.
	class UILstr {
	public:
		/*
		* A child UI component has triggered an event.
		* @param unsigned int cID: The ID of the component.
		* @param unsigned int eID: The ID of the event.
		*/
		virtual void cmptEvent(unsigned int, unsigned int) = 0;
		/*
		* @return The shader used for drawing children of this UI listener.
		*/
		virtual Shader& getShader() = 0;
		/*
		* @return The font used for drawing children of this UI listener.
		*/
		virtual const Font& getFont() const = 0;
	};

	// An abstract user interface component.
	class UICmpt : public InputLstr {
	public:
		/*
		* Free this component's memory.
		*/
		virtual void destroy() = 0;
		/*
		* @return The parent UI listener of this component.
		*/
		inline virtual UILstr& getParent() {
			return *parent_;
		}
		/*
		* @return The ID number of this component.
		*/
		inline virtual unsigned int getID() const {
			return ID_;
		}
		/*
		* @return The position of this component.
		*/
		virtual glm::vec3 getPos() const = 0;
		/*
		* @param const glm::vec3& pos: The device-normalized 2D position and depth of
		* this component.
		*/
		virtual void setPos(const glm::vec3&) = 0;
		/*
		* @return The dimensions of this component.
		*/
		virtual glm::vec2 getDims() const = 0;
		/*
		* @param const glm::vec2& dims: The device-normalized 2D dimensions of this
		* component.
		*/
		virtual void setDims(const glm::vec2&) = 0;

	protected:
		friend class UIGroup;

		// The parent UI listener of this component.
		UILstr* parent_ = 0;
		// The ID number of this component.
		unsigned int ID_ = 0;

		/*
		* Draw this component's graphics using the App's renderer.
		*/
		virtual void draw() = 0;
		/*
		* Process user input to this component.
		*/
		virtual void processInput() = 0;
		/*
		* Update this component's logic.
		* @param float dt: Timestep since last update.
		*/
		virtual void update(float) = 0;
	};

	// A group of user interface components with a parent scene.
	class UIGroup : public UILstr {
	public:
		/*
		* Initialize this UI group.
		* @param Scene& parent: The parent scene of this group.
		* @param unsigned int ID: The ID number of this group in the parent scene.
		* @param Shader& shader: The shader to use to draw components in this group.
		* @param const Font& font: The font to use to draw components in this group.
		*/
		void init(Scene&, unsigned int, Shader&, const Font&);
		/*
		* Add a UI component to this group.
		* @return Whether the group was changed.
		*/
		bool addCmpt(UICmpt&);
		/*
		* Remove a UI component from this group.
		* @return Whether the group was changed.
		*/
		bool removeCmpt(UICmpt&);
		/*
		* Draw all this group's child components.
		*/
		void draw();
		/*
		* Process user input to all this group's child components.
		*/
		void processInput();
		/*
		* Process component events in this group.
		* @param unsigned int cID: The ID of the component.
		* @param unsigned int eID: The ID of the event.
		*/
		void cmptEvent(unsigned int, unsigned int) override;
		/*
		* Update the logic of all this group's child components.
		* @param float dt: Timestep since the last update.
		*/
		void update(float);
		/*
		* Free this group's memory and that of all its components.
		*/
		void destroy();
		/*
		* @return This group's parent scene.
		*/
		inline Scene& getParent() {
			return *parent_;
		}
		/*
		* @return This group's ID number in its parent scene.
		*/
		inline unsigned int getID() const {
			return ID_;
		}
		/*
		* @return This group's shader.
		*/
		inline Shader& getShader() override {
			return *shader_;
		}
		/*
		* @param Shader& shader: This group's shader.
		*/
		inline void setShader(Shader& shader) {
			shader_ = &shader;
		}
		/*
		* @return This group's font.
		*/
		inline const Font& getFont() const override {
			return font_;
		}
		/*
		* @param const Font& font: This group's font.
		*/
		inline void setFont(const Font& font) {
			font_ = font;
		}
		/*
		* @return Whether this group's components are enabled.
		*/
		inline bool isEnabled() const {
			return enabled_;
		}
		/*
		* @param bool enabled: Whether this group's components are enabled.
		*/
		void setEnabled(bool enabled);
		/*
		* @return Whether this group's components are visible.
		*/
		inline bool isVisible() const {
			return visible_;
		}
		/*
		* @param bool visible: Whether this group's components are visible.
		*/
		inline void setVisible(bool visible) {
			visible_ = visible;
		}

	private:
		// This group's parent scene.
		Scene* parent_ = 0;
		// This group's ID number.
		unsigned int ID_ = 0;
		// Pointers to the components in this group.
		std::vector<UICmpt*> cmpts_;
		// The next ID to assign to a component passed to addCmpt().
		unsigned int nextID_ = 0;
		// Pointer to this group's shader.
		Shader* shader_ = 0;
		// This group's font.
		Font& font_ = Renderer::NO_FONT;
		// Whether this group's components are enabled.
		bool enabled_ = true;
		// Whether this group's components are visible.
		bool visible_ = true;
		// The dimensions of the window in the latest update().
		glm::ivec2 prevWindowDims_ = glm::ivec2();
	};

	// Text label UI component.
	class Label : public UICmpt {
	public:
		/*
		* Initialize this label's memory.
		* @param const glm::vec3& pos: The device-normalized position of this label.
		* @param const glm::vec2& dims: The device-normalized dimensions of this label.
		* @param const GLuint texture: The texture ID for this label's background.
		* @param const std::string& text: The text to display in this label.
		* @param float textScale: The scale to display this label's text at.
		* @param const glm::vec3& textColor: The color to display this label's text in.
		* @param Alignment hAlign: The horizontal alignment of this label's text.
		* @param Alignment vAlign: The vertical alignment of this label's text.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const std::string&,
			float, const glm::vec3&, Alignment = ASC_ALIGN_CENTER, Alignment = ASC_ALIGN_CENTER);
		/*
		* Free this label's memory.
		*/
		virtual void destroy() override;
		/*
		* @return This label's position and depth.
		*/
		virtual glm::vec3 getPos() const override;
		/*
		* @param const glm::vec3& pos: The device-normalized position for this label.
		*/
		virtual void setPos(const glm::vec3&) override;
		/*
		* @return This label's dimensions.
		*/
		virtual glm::vec2 getDims() const override;
		/*
		* @param const glm::vec2& dims: The device-normalized dimensions of this label.
		*/
		virtual void setDims(const glm::vec2&) override;
		/*
		* @return The text in this label.
		*/
		inline virtual const std::string& getText() const {
			return text_;
		}
		/*
		* @param const std::string& text: The text to display in this label.
		*/
		inline virtual void setText(const std::string& text) {
			text_ = text;
		}
		/*
		* @return The scale this label displays text at.
		*/
		inline virtual float getTextScale() const {
			return textScale_;
		}
		/*
		* @param float textScale: The scale to display this label's text at.
		*/
		inline virtual void setTextScale(float textScale) {
			textScale_ = textScale;
		}
		/*
		* @return The color this label displays text in.
		*/
		inline virtual const glm::vec3& getTextColor() const {
			return textColor_;
		}
		/*
		* @param const glm::vec3& textColor: The color to display this label's text in.
		*/
		inline virtual void setTextColor(const glm::vec3& textColor) {
			textColor_ = textColor;
		}
		/*
		* @return The horizontal alignment of this label's text.
		*/
		inline virtual Alignment getHAlign() const {
			return hAlign_;
		}
		/*
		* @param Alignment hAlign: The horizontal alignment for this label's text.
		*/
		inline virtual void setHAlign(Alignment hAlign) {
			hAlign_ = hAlign;
		}
		/*
		* @return The vertical alignment of this label's text.
		*/
		inline virtual Alignment getVAlign() const {
			return vAlign_;
		}
		/*
		* @param Alignment vAlign: The vertical alignment for this label's text.
		*/
		inline virtual void setVAlign(Alignment vAlign) {
			vAlign_ = vAlign;
		}

	protected:
		// Sprite used for background texturing of this label.
		Sprite bgSprite_;
		// The text to display in this label.
		std::string text_ = "";
		// The scale to display this label's text at.
		float textScale_ = 1.0f;
		// The color to display this label's text in.
		glm::vec3 textColor_ = glm::vec3();
		// The horizontal alignment of this label's text.
		Alignment hAlign_ = ASC_ALIGN_CENTER;
		// The vertical alignment of this label's text.
		Alignment vAlign_ = ASC_ALIGN_CENTER;

		/*
		* Draw this label's background sprite and text.
		*/
		virtual void draw() override;
		/*
		* Do not process user input to labels.
		*/
		virtual void processInput() override {}
		/*
		* Do not update logic for labels.
		* @param float dt: Timestep since the last update.
		*/
		virtual void update(float) override {}
	};

	// Clickable button UI component.
	class Button : public Label {
	public:
		// Event ID for unselected (moused off of) button.
		const static unsigned int UNSELECTED_EVENT = 0;
		// Event ID for selected (moused over) button.
		const static unsigned int SELECTED_EVENT = 1;
		// Event ID for clicked button.
		const static unsigned int CLICKED_EVENT = 2;

		/*
		* Initialize this button's memory.
		* @param const glm::vec3& pos: The device-normalized position of this button.
		* @param const glm::vec2& dims: The device-normalized dimensions of this button.
		* @param const GLuint texture: The texture ID for this button's background.
		* @param const std::string& text: The text to display in this button.
		* @param float textScale: The scale to display this button's text at.
		* @param const glm::vec3& textColor: The color to display this button's text in.
		* @param Alignment hAlign: The horizontal alignment of this button's text.
		* @param Alignment vAlign: The vertical alignment of this button's text.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const std::string&,
			float, const glm::vec3&, Alignment = ASC_ALIGN_CENTER, Alignment = ASC_ALIGN_CENTER) override;
		/*
		* Free this button's memory.
		*/
		virtual void destroy() override;
		/*
		* @return Whether this button is currently selected (moused over).
		*/
		inline virtual bool isSelected() const {
			return selected_;
		}
		/*
		* @param bool selected: Whether this button is selected.
		*/
		virtual void setSelected(bool);
		/*
		* @return Whether this button has been clicked.
		*/
		inline virtual bool isClicked() const {
			return clicked_;
		}
		/*
		* @param bool clicked: Whether this button is clicked.
		*/
		virtual void setClicked(bool);

	protected:
		friend class Cycle;

		// The texture coordinates for unselected button image.
		static glm::vec4 UNSELECTED_COORDS;
		// The texture coordinates for selected button image.
		static glm::vec4 SELECTED_COORDS;
		// The texture coordinates for clicked button image.
		static glm::vec4 CLICKED_COORDS;
		// Whether this button is currently selected (moused over).
		bool selected_ = false;
		// Whether this button has been clicked.
		bool clicked_ = false;

		/*
		* Process user input to this button.
		*/
		virtual void processInput() override;
	};

	// Toggle button UI component.
	class Switch : public Label {
	public:
		// Event ID for switch turned off.
		const static unsigned int OFF_EVENT = 0;
		// Event ID for switch turned on.
		const static unsigned int ON_EVENT = 1;

		/*
		* Initialize this switch's memory.
		* @param const glm::vec3& pos: The device-normalized position of this switch.
		* @param const glm::vec2& dims: The device-normalized dimensions of this switch.
		* @param const GLuint texture: The texture ID for this switch's background.
		* @param const std::string& text: The text to display in this switch.
		* @param float textScale: The scale to display this switch's text at.
		* @param const glm::vec3& textColor: The color to display this switch's text in.
		* @param Alignment lAlign: The alignment of this switch's label about its background.
		* @param Alignment lHAlgin: The horizontal alignment of the label's text in its bounds.
		* @param Alignment lVAlign: The vertical alignment of the label's text in its bounds.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const std::string&,
			float, const glm::vec3&, Alignment = ASC_ALIGN_TOP, Alignment = ASC_ALIGN_LEFT,
			Alignment = ASC_ALIGN_CENTER);
		/*
		* Free this switch's memory.
		*/
		virtual void destroy() override;
		/*
		* @return Whether this switch is currently turned on.
		*/
		inline virtual bool isOn() const {
			return on_;
		}
		/*
		* @param bool on: Whether this switch is turned on.
		*/
		virtual void setOn(bool);
		/*
		* @return The alignment of this switch's label about its background.
		*/
		inline virtual Alignment getLAlign() const {
			return lAlign_;
		}
		/*
		* @param Alignment lAlign: The alignment for this switch's label about its background.
		*/
		inline virtual void setLAlign(Alignment lAlign) {
			lAlign_ = lAlign;
		}

	protected:
		// The texture coordinates for turned off switch image.
		static glm::vec4 OFF_COORDS;
		// The texture coordinates for turned on switch image.
		static glm::vec4 ON_COORDS;
		// Whether this switch is currently turned on.
		bool on_ = false;
		// The alignment of this switch's label about its background.
		Alignment lAlign_ = ASC_ALIGN_TOP;

		/*
		* Draw this switch's label and background.
		*/
		virtual void draw() override;
		/*
		* Process user input to this switch.
		*/
		virtual void processInput() override;
	};

	// Slider UI component.
	class Slider : public Label {
	public:
		// Event ID for updated slider value.
		const static unsigned int SET_EVENT = 0;

		/*
		* Initialize this slider's memory.
		* @param const glm::vec3& pos: The device-normalized position of this slider.
		* @param const glm::vec2& dims: The device-normalized dimensions of this slider.
		* @param const GLuint texture: The texture ID for this slider's background.
		* @param const GLuint cursorTexture: The texture ID for this slider's cursor.
		* @param float cursorWidth: The width in pixels for this slider's cursor.
		* @param const std::string& text: The text to display in this slider.
		* @param float textScale: The scale to display this slider's text at.
		* @param const glm::vec3& textColor: The color to display this slider's text in.
		* @param Alignment lAlign: The alignment of this slider's label about its background.
		* @param Alignment lHAlign: The horizontal alignment of the label's text in its bounds.
		* @param Alignment lVAlign: The vertical alignment of this label's text in its bounds.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const GLuint, float,
			const std::string&, float, const glm::vec3&, Alignment = ASC_ALIGN_TOP,
			Alignment = ASC_ALIGN_LEFT, Alignment = ASC_ALIGN_CENTER);
		/*
		* Free this slider's memory.
		*/
		virtual void destroy() override;
		/*
		* @return The current value of this slider (0.0f - 1.0f).
		*/
		inline virtual float getValue() const {
			return value_;
		}
		/*
		* @param float value: The value for this slider (0.0f - 1.0f).
		*/
		inline virtual void setValue(float value) {
			value_ = value;
			parent_->cmptEvent(ID_, SET_EVENT);
		}

	protected:
		// The alignment of this slider's label about its background texture.
		Alignment lAlign_ = ASC_ALIGN_TOP;
		// Sprite used to place the slider's cursor texture.
		Sprite cursor_;
		// The current value of this slider.
		float value_ = 0.0f;
		// Whether this slider is currently selected.
		bool selected_ = false;

		/*
		* Draw this slider's background, label, and cursor.
		*/
		virtual void draw() override;
		/*
		* Process user input to this slider.
		*/
		virtual void processInput() override;
		/*
		* Update this slider's logic.
		* @param float dt: Timestep since last update.
		*/
		virtual void update(float) override;
	};

	// Cycle / carousel UI component.
	class Cycle : public Label, UILstr {
	public:
		// Event ID for updated cycle value.
		const static unsigned int SET_EVENT = 0;

		/*
		* Initialize this cycle's memory.
		* @param const glm::vec3& pos: The device-normalized position for this cycle.
		* @param const glm::vec2& dims: The device-normalized dimensions for this cycle.
		* @param const GLuint texture: The background texture for this cycle.
		* @param const GLuint buttonTexture: The texture for this cycle's previous and next buttons.
		* @param const std::vector<std::string>& values: The initial possible values for this cycle.
		* @param const std::string& labelText: The text to display in this cycle's label.
		* @param const std::string& prevButtonText: The text to display in this cycle's previous value button.
		* @param const std::string& nextButtonText: The text to display in this cycle's next value button.
		* @param float textScale: The scale to display this cycle's text at.
		* @param const glm::vec3& textColor: The color to display this cycle's text in.
		* @param Alignment hAlign: The horizontal alignment for this cycle's value text.
		* @param Alignment vAlign: The vertical alignment for this cycle's value text.
		* @param Alignment bAlign: The left/right alignment for this cycle's prev/next buttons.
		* @param Alignment lAlign: The alignment for this cycle's label about its background.
		* @param Alignment lHAlign: The horizontal alignment for this cycle's label text in its bounds.
		* @param Alignment lVAlign: The vertical alignment for this cycle's label text in its bounds.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const GLuint,
			const std::vector<std::string>&, const std::string&, const std::string&,
			const std::string&, float, const glm::vec3&, Alignment = ASC_ALIGN_CENTER,
			Alignment = ASC_ALIGN_CENTER, Alignment = ASC_ALIGN_RIGHT, Alignment = ASC_ALIGN_TOP,
			Alignment = ASC_ALIGN_LEFT, Alignment = ASC_ALIGN_CENTER);
		/*
		* Free this cycle's memory.
		*/
		virtual void destroy() override;
		/*
		* Add a possible value to this cycle.
		* @param const std::string& value: The value to add.
		*/
		virtual void addValue(const std::string&);
		/*
		* Remove a value from this cycle by its index.
		* @param int index: The index of the value to remove.
		*/
		virtual void removeValue(int);
		/*
		* Get the index of a string in the set of values for this cycle.
		* @param const std::string& value: The value to find.
		* @return The first index that this value appears at or -1.
		*/
		virtual int getIndexOf(const std::string&) const;
		/*
		* @return The current value displayed by this cycle.
		*/
		virtual const std::string& getValue() const;
		/*
		* @return The index of the current value displayed by this cycle.
		*/
		inline virtual int getIndex() const {
			return value_;
		}
		/*
		* @param const std::string& text: The text to display in this cycle's value,
		* adds the value if not already present.
		*/
		virtual void setText(const std::string&) override;
		/*
		* @return The position of this cycle considering button alignment.
		*/
		virtual glm::vec3 getPos() const override;
		/*
		* @param const glm::vec3& pos: The device-normalized position for this cycle.
		*/
		virtual void setPos(const glm::vec3&) override;
		/*
		* @return The dimensions of this cycle including buttons.
		*/
		virtual glm::vec2 getDims() const override;
		/*
		* @param const glm::vec2& dims: The device-normalized dimensions of this cycle
		* including buttons.
		*/
		virtual void setDims(const glm::vec2&) override;
		/*
		* A button has triggered an event in this cycle.
		* @param unsigned int cID: The button's ID number.
		* @param unsigned int eID: The event ID number.
		*/
		virtual void cmptEvent(unsigned int, unsigned int) override;
		/*
		* @return The shader of this cycle's parent UI group.
		*/
		inline virtual Shader& getShader() override {
			return parent_->getShader();
		}
		/*
		* @return The font of this cycle's parent UI group.
		*/
		inline virtual const Font& getFont() const override {
			return parent_->getFont();
		}

	protected:
		// The horizontal alignment of this cycle's buttons about its background.
		Alignment bAlign_ = ASC_ALIGN_RIGHT;
		// The previous value button for this cycle.
		Button prevB_;
		// The next value button for this cycle.
		Button nextB_;
		// The possible values of this cycle.
		std::vector<std::string> values_;
		// The current index of the value of this cycle.
		int value_ = -1;
		// The text to display in the label about this cycle's background.
		std::string labelText_ = "";
		// The alignment of this cycle's label about its background.
		Alignment lAlign_ = ASC_ALIGN_TOP;
		// The horizontal alignment of the label's text in its bounds.
		Alignment lHAlign_ = ASC_ALIGN_LEFT;
		// The vertical alignment of the label's text in its bounds.
		Alignment lVAlign_ = ASC_ALIGN_CENTER;
		
		/*
		* Draw this cycle's previous and next buttons, background, value, and label.
		*/
		virtual void draw() override;
		/*
		* Process user input to this cycle.
		*/
		virtual void processInput() override;
		/*
		* Update this cycle's logic.
		* @param float dt: Timestep since last update.
		*/
		virtual void update(float) override;
	};

	// Text input field UI component.
	class TextBox : public Label {
	public:
		// Event ID for unselected (clicked out of) text box.
		const static unsigned int UNSELECTED_EVENT = 0;
		// Event ID for selected (clicked) text box.
		const static unsigned int SELECTED_EVENT = 1;
		// Event ID for text entered into text box.
		const static unsigned int ENTERED_EVENT = 2;

		/*
		* Initialize this text box's memory.
		* @param const glm::vec3& pos: The device-normalized position for this text box.
		* @param const glm::vec2& dims: The device-normalized dimensions for this text box.
		* @param const GLuint texture: The background texture ID for this text box.
		* @param const GLuint cursorTexture: The texture ID for this text box's cursor.
		* @param float cursorWidth: The width in pixels for this text box's cursor.
		* @param const std::string& labelText: The text to display about this text box's background.
		* @param const std::string& text: The initial text to display in this text box.
		* @param float textScale: The scale to display this text box's text at.
		* @param const glm::vec3& textColor: The color to display this text box's text in.
		* @param int maxChars: The maximum number of characters allowed in this text box (-1 for inf).
		* @param const std::string& allowedChars: The set of characters allowed in this text box
		* (empty string for all).
		* @param Alignment vAlign: The vertical alignment of this text box's main text.
		* @param Alignment lAlign: The alignment of this text box's label about its background.
		* @param Alignment lHAlign: The horizontal alignment of the label's text in its bounds.
		* @param Alignment lVAlign: The vertical alignment of this label's text in its bounds.
		*/
		virtual void init(const glm::vec3&, const glm::vec2&, const GLuint, const GLuint, float,
			const std::string&, const std::string&, float, const glm::vec3&, int = -1,
			const std::string& = "", Alignment = ASC_ALIGN_CENTER, Alignment = ASC_ALIGN_TOP,
			Alignment = ASC_ALIGN_LEFT, Alignment = ASC_ALIGN_CENTER);
		/*
		* @param const glm::vec3& pos: The device-normalized position for this text box.
		*/
		virtual void setPos(const glm::vec3&) override;
		/*
		* @param const std::string& text: The text for this text box's content.
		*/
		virtual void setText(const std::string&) override;
		/*
		* Callback for a character typed into this text box.
		* @param char c: The character typed.
		*/
		virtual void charTyped(char c) override;

	protected:
		// Texture coordinates for the unselected text box.
		static glm::vec4 UNSELECTED_COORDS;
		// Texture coordinates for the selected text box.
		static glm::vec4 SELECTED_COORDS;
		// Whether this text box is selected / clicked on.
		bool selected_ = false;
		// The text to display about this text box's background.
		std::string labelText_ = "";
		// The current position of the start of this box's main text.
		float textPos_ = 0.0f;
		// Sprite used to display this text box's cursor.
		Sprite cursor_;
		// The position of the cursor in the text box's main text.
		unsigned int cursorPos_ = 0;
		// Time tracking variable for repeating inputs.
		float timer_ = 0.0f;
		// Maximum number of characters allowed in this text box.
		int maxChars_ = -1;
		// Set of characters allowed in this text box.
		std::string allowedChars_ = "";
		// Alignment of this text box's label about its background.
		Alignment lAlign_ = ASC_ALIGN_TOP;
		// Horizontal alignment of the label's text in its bounds.
		Alignment lHAlign_ = ASC_ALIGN_LEFT;
		// Vertical alignment of the label's text in its bounds.
		Alignment lVAlign_ = ASC_ALIGN_CENTER;

		/*
		* Draw this text box's cursor, text, and label.
		*/
		virtual void draw() override;
		/*
		* Process user input to this text box.
		*/
		virtual void processInput() override;
		/*
		* Upate this text box's logic.
		* @param float dt: Timestep since last update.
		*/
		virtual void update(float) override;
	};
}

#endif