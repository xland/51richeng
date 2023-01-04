#include "ViewBase.h"
void ViewBase::show() {
	document->Show();
}
void ViewBase::hide() {
	if (document != nullptr) {
		document->Hide();
	}	
}