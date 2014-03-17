
// Define.h

#ifndef DEFINE_H_
#define DEFINE_H_

// タッチ
namespace touch {
    enum Type {
        None,
        Began,
        Moved,
        Ended,
        Cancelled,
        Num
    };
}

// ボタン
namespace button {
	enum Type {
		None,
	    Slow,
	    Jump,
	    Dash,
	    Drink,	
	    Num
	};
}

// レイヤー
namespace layer {
	enum Type {
		bgBack,
		player,
		object,
		bgFront,
		snow,
		ui,
		debug,
        Num
	};
}

#endif