struct Button_State {
	bool is_down;
	bool changed;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_W,
	BUTTON_S,
	BUTTON_A,
	BUTTON_D,

	BUTTON_ENTER,
	BUTTON_SPACE,

	BUTTON_COUNT, // Should be last item
};

struct Input {
	Button_State buttons[BUTTON_COUNT];
};