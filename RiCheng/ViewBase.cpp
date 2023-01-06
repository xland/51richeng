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
	totalHeight -= 50;
	float h = 24 * position / totalHeight;
	float m = 60 * (h - (int)h);
	if (m > 56) {
		h += 1;
		m = 0;
	}
	else if (m < 4) {
		m = 0;
	}
	auto hStr = h < 10 ? std::format("0{0}", (int)h) : std::to_string((int)h);
	auto mStr = m < 10 ? std::format("0{0}", (int)m) : std::to_string((int)m);
	return std::format("{0}:{1}", hStr, mStr);
}
void ViewBase::RemoveDocumentListener() {
	document->RemoveEventListener(Rml::EventId::Mousemove, this);
	document->RemoveEventListener(Rml::EventId::Mouseup, this);
}
void ViewBase::AddDocumentListener() {
	document->AddEventListener(Rml::EventId::Mousemove, this);
	document->AddEventListener(Rml::EventId::Mouseup, this);
}