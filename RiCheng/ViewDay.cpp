#include "ViewDay.h"
#include <format>

ViewDay::ViewDay() {
	auto context = Rml::GetContext("main");
	document = context->LoadDocument("ui/viewDay.rml");
	document->SetId("viewDay");
	document->SetProperty(Rml::PropertyId::Top, Rml::Property(50, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::Left, Rml::Property(400, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::Right, Rml::Property(0, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::Bottom, Rml::Property(0, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::ZIndex, Rml::Property(2, Rml::Property::NUMBER));
	document->Show();
	Rml::ElementList list;
	document->QuerySelectorAll(list, ".todoItem > div");
	for (auto& ele : list)
	{
		ele->AddEventListener(Rml::EventId::Mousedown, this);
	}
}
void ViewDay::updateTargetTime() {
	auto top = targetEle->GetProperty(Rml::PropertyId::Top)->value.Get<int>();
	auto bottom = top + targetEle->GetClientHeight();
	auto totalHeight = targetEle->GetParentNode()->GetClientHeight();
	std::string endTime;
	auto func = [&totalHeight](int p) {
		double h1 = 24 * p / totalHeight;
		double m1 = 60 * (h1 - (int)h1);
		if (m1 > 56) {
			h1 += 1;
			m1 = 0;
		}
		else if (m1 < 4) {
			m1 = 0;
		}
		auto hStr = h1 < 10 ? ("0" + std::to_string((int)h1)) : std::to_string((int)h1);
		auto mStr = m1 < 10 ? ("0" + std::to_string((int)m1)) : std::to_string((int)m1);
		return std::format("{0}:{1}", hStr, mStr);
	};
	std::string timeStr = std::format("{0}-{1}", func(top), func(bottom));
	targetEle->GetChild(1)->GetChild(0)->SetInnerRML(timeStr);
}
void ViewDay::processMouseDown(const std::string& className, const Rml::Vector2f& mousePoint) {
	if (className == "todoContent") {
		targetEleHeight = targetEle->GetClientHeight();
		dragType = 1;
		mousePointTopSpan = mousePoint.y - targetEle->GetAbsoluteTop() + 50;//50是标题栏的高度
		mousePointLeftSpan = mousePoint.x - targetEle->GetAbsoluteLeft() + 499; //左边距
		document->AddEventListener(Rml::EventId::Mousemove, this);
		document->AddEventListener(Rml::EventId::Mouseup, this);
	}
	else if (className == "todoDragBottom") {
		targetEleHeight = targetEle->GetClientHeight();
		dragType = 2;
		document->AddEventListener(Rml::EventId::Mousemove, this);
		document->AddEventListener(Rml::EventId::Mouseup, this);
	}
	else if (className == "todoDragTop") {
		targetEleHeight = targetEle->GetClientHeight();
		dragType = 3;
		document->AddEventListener(Rml::EventId::Mousemove, this);
		document->AddEventListener(Rml::EventId::Mouseup, this);
	}
}
void ViewDay::processMouseMove(const Rml::Vector2f& mousePoint) {
	//todo 动态颜色
	if (dragType == 1) {
		auto mouseY = mousePoint.y;
		auto yPoint = mouseY - mousePointTopSpan;
		if (yPoint < 3) yPoint = 3;
		auto bottom = targetEle->GetParentNode()->GetClientHeight() - yPoint - targetEleHeight;
		targetEle->SetProperty(Rml::PropertyId::Top, Rml::Property(yPoint, Rml::Property::PX));
		targetEle->SetProperty(Rml::PropertyId::Bottom, Rml::Property(bottom, Rml::Property::PX));
		if (false) {
			auto xPoint = mousePoint.x - mousePointLeftSpan;
			targetEle->SetProperty(Rml::PropertyId::Left, Rml::Property(xPoint, Rml::Property::PX));
		}
	}
	else if (dragType == 2) {
		auto height = mousePoint.y - 50;
		auto bottom = targetEle->GetParentNode()->GetClientHeight() - height - 3;
		if (bottom < 3) {
			bottom = 3;
			document->RemoveEventListener(Rml::EventId::Mousemove, this);
			document->RemoveEventListener(Rml::EventId::Mouseup, this);
		}
		if (targetEle->GetParentNode()->GetClientHeight() - targetEle->GetProperty(Rml::PropertyId::Top)->value.Get<int>() - bottom <= 20) {
			bottom = targetEle->GetParentNode()->GetClientHeight() - targetEle->GetProperty(Rml::PropertyId::Top)->value.Get<int>() - 20;
		}
		targetEle->SetProperty(Rml::PropertyId::Bottom, Rml::Property(bottom, Rml::Property::PX));
	}
	else if (dragType == 3) {
		auto top = mousePoint.y - 50 - 3;
		if (top < 3) {
			top = 3;
		}
		if (targetEle->GetParentNode()->GetClientHeight() - targetEle->GetProperty(Rml::PropertyId::Bottom)->value.Get<int>() - top <= 20) {
			top = targetEle->GetParentNode()->GetClientHeight() - targetEle->GetProperty(Rml::PropertyId::Bottom)->value.Get<int>() - 20;
		}
		targetEle->SetProperty(Rml::PropertyId::Top, Rml::Property(top, Rml::Property::PX));

	}
	updateTargetTime();
}

void ViewDay::ProcessEvent(Rml::Event& event) {
	auto ele = event.GetCurrentElement();
	auto eleId = ele->GetId();
	switch (event.GetId())
	{
	case Rml::EventId::Mousedown: {
		targetEle = ele->GetParentNode();
		processMouseDown(ele->GetClassNames(), event.GetUnprojectedMouseScreenPos());
		break;
	}
	case Rml::EventId::Mousemove: {
		if (ele == document) {
			processMouseMove(event.GetUnprojectedMouseScreenPos());
		}
		break;
	}
	case Rml::EventId::Mouseup: {
		if (ele == document) {
			document->RemoveEventListener(Rml::EventId::Mousemove, this);
			document->RemoveEventListener(Rml::EventId::Mouseup, this);
			if (false) {
				auto left = targetEle->GetAbsoluteLeft() - 499;
				int index = left / (targetEle->GetParentNode()->GetClientWidth() / 7);
				targetEle->SetProperty(Rml::PropertyId::Left, Rml::Property(14.2857 * index, Rml::Property::PERCENT));
			}
		}
		break;
	}
	default:
		break;
	}
}
