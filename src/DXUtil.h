#pragma once

// General DirectX utility/helper functionality should go here.

#define SAFE_RELEASE(x) if (x) { x->Release(); x = nullptr; }
