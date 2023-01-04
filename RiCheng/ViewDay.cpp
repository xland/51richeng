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

void ViewDay::ProcessEvent(Rml::Event& event) {
	auto ele = event.GetCurrentElement();
	auto eleId = ele->GetId();
	switch (event.GetId())
	{
	case Rml::EventId::Mousedown: {
		auto eleClassName = ele->GetClassNames();
		if (eleClassName == "todoContent") {
			targetEle = ele->GetParentNode();
			targetEleHeight = targetEle->GetClientHeight();
			dragType = 1;
			mousePointTopSpan = event.GetUnprojectedMouseScreenPos().y - targetEle->GetAbsoluteTop() + 50;//50是标题栏的高度
			mousePointLeftSpan = event.GetUnprojectedMouseScreenPos().x - targetEle->GetAbsoluteLeft() + 499; //左边距
			document->AddEventListener(Rml::EventId::Mousemove, this);
			document->AddEventListener(Rml::EventId::Mouseup, this);
		}
		else if (eleClassName == "todoDragBottom") {
			targetEle = ele->GetParentNode();
			targetEleHeight = targetEle->GetClientHeight();
			dragType = 2;
			document->AddEventListener(Rml::EventId::Mousemove, this);
			document->AddEventListener(Rml::EventId::Mouseup, this);
		}
		else if (eleClassName == "todoDragTop") {
			targetEle = ele->GetParentNode();
			targetEleHeight = targetEle->GetClientHeight();
			dragType = 3;
			document->AddEventListener(Rml::EventId::Mousemove, this);
			document->AddEventListener(Rml::EventId::Mouseup, this);
		}
		break;
	}
	case Rml::EventId::Mousemove: {
		if (ele == document) {
			//todo 动态颜色
			if (dragType == 1) {
				auto mouseY = event.GetUnprojectedMouseScreenPos().y;
				auto yPoint = mouseY - mousePointTopSpan;
				if (yPoint < 3) yPoint = 3;
				auto bottom = targetEle->GetParentNode()->GetClientHeight() - yPoint - targetEleHeight;
				targetEle->SetProperty(Rml::PropertyId::Top, Rml::Property(yPoint, Rml::Property::PX));
				targetEle->SetProperty(Rml::PropertyId::Bottom, Rml::Property(bottom, Rml::Property::PX));
				if (viewMode == 2) {
					auto xPoint = event.GetUnprojectedMouseScreenPos().x - mousePointLeftSpan;
					targetEle->SetProperty(Rml::PropertyId::Left, Rml::Property(xPoint, Rml::Property::PX));
				}
			}
			else if (dragType == 2) {
				auto height = event.GetUnprojectedMouseScreenPos().y - 50;
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
				auto top = event.GetUnprojectedMouseScreenPos().y - 50 - 3;
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
		break;
	}
	case Rml::EventId::Mouseup: {
		if (ele == document) {
			document->RemoveEventListener(Rml::EventId::Mousemove, this);
			document->RemoveEventListener(Rml::EventId::Mouseup, this);
			if (viewMode == 2 && dragType == 1) {
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
