#include "ViewBase.h"
#include <format>
void ViewBase::show() {
	document->Show();
}
void ViewBase::hide() {
	if (document != nullptr) {
		document->Hide();
	}	
}
std::string ViewBase::PositionToTime(float totalHeight, float position) {
	double h = 24 * position / totalHeight;
	double m = 60 * (h - (int)h);
	if (m > 56) {
		h += 1;
		m = 0;
	}
	else if (m < 4) {
		m = 0;
	}
	auto hStr = h < 10 ? ("0" + std::to_string((int)h)) : std::to_string((int)h);
	auto mStr = m < 10 ? ("0" + std::to_string((int)m)) : std::to_string((int)m);
	return std::format("{0}:{1}", hStr, mStr);
}