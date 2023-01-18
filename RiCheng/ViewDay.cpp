#include "ViewDay.h"
#include "ResourceHelper.h"
#include <format>


ViewDay::ViewDay() {
	auto context = Rml::GetContext("windowMain");
	document = context->LoadDocument("ui/viewDay.rml");
	document->SetId("viewDay");
	document->SetProperty(Rml::PropertyId::Top, Rml::Property(50, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::Left, Rml::Property(400, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::ZIndex, Rml::Property(1, Rml::Property::NUMBER));
	document->AddEventListener(Rml::EventId::Resize, this);
	document->Show();
	for (size_t i = 0; i < 2; i++)
	{
		createTodoEle(i + 50, i+50, i);
	}
}

void ViewDay::createTodoEle(int top,int bottom,int index) {	
	auto templateEle = document->GetElementById("todoItemTemplate");
	Rml::ElementPtr ele = templateEle->Clone();
	ele->SetProperty(Rml::PropertyId::Top, Rml::Property(top, Rml::Property::PX));
	ele->SetProperty(Rml::PropertyId::Bottom, Rml::Property(bottom, Rml::Property::PX));
	ele->SetProperty(Rml::PropertyId::ZIndex, Rml::Property(index+3, Rml::Property::NUMBER));
	auto className = std::format("todoItem{0}", index % 6);
	ele->SetClass(className, true);
	ele->GetChild(0)->AddEventListener(Rml::EventId::Mousedown, this);
	ele->GetChild(1)->AddEventListener(Rml::EventId::Mousedown, this);
	ele->GetChild(2)->AddEventListener(Rml::EventId::Mousedown, this);
	templateEle->GetPreviousSibling()->GetFirstChild()->AppendChild(std::move(ele));
}

void ViewDay::activeTodoEle(Rml::Element* ele) {
	auto container = document->GetElementById("todoContainer");
	auto oldIndex = ele->GetProperty(Rml::PropertyId::ZIndex)->value.Get<int>();
	auto item = container->GetFirstChild();
	int topIndex = 3;
	while (item != nullptr)
	{
		if (item != ele) {
			auto itemIndex = item->GetProperty(Rml::PropertyId::ZIndex)->value.Get<int>();
			if (itemIndex > oldIndex) {
				item->SetProperty(Rml::PropertyId::ZIndex, Rml::Property(itemIndex - 1, Rml::Property::NUMBER));
			}
		}
		topIndex += 1;
		item = item->GetNextSibling();
	}
	if (topIndex == oldIndex) return;
	ele->SetProperty(Rml::PropertyId::ZIndex, Rml::Property(topIndex, Rml::Property::NUMBER));
}

//void ViewDay::updateTargetTime() {
//	auto top = targetEle->GetProperty(Rml::PropertyId::Top)->value.Get<float>();
//	auto bottom = top + targetEle->GetClientHeight();
//	auto totalHeight = targetEle->GetParentNode()->GetClientHeight();
//	std::string timeStr = std::format("{0}-{1}", PositionToTime(totalHeight,top), PositionToTime(totalHeight,bottom));
//	targetEle->GetChild(1)->GetChild(0)->SetInnerRML(timeStr);
//}
void ViewDay::processMouseDown(const std::string& className, const Rml::Vector2f& mousePoint) {
	if (className == "todoContent") {
		targetEleHeight = targetEle->GetClientHeight();
		dragType = 1;
		mousePointTopSpan = mousePoint.y - targetEle->GetAbsoluteTop() + 50;//50是标题栏的高度
		//mousePointLeftSpan = mousePoint.x - targetEle->GetAbsoluteLeft() + 499; //左边距
		AddDocumentListener();
	}
	else if (className == "todoDragBottom") {
		targetEleHeight = targetEle->GetClientHeight();
		dragType = 2;
		AddDocumentListener();
	}
	else if (className == "todoDragTop") {
		targetEleHeight = targetEle->GetClientHeight();
		dragType = 3;
		AddDocumentListener();
	}
}
void ViewDay::processMouseMove(const Rml::Vector2f& mousePoint) {
	auto totalHeight = document->GetClientHeight();
	//todo 动态颜色
	if (dragType == 1) { //移动
		auto top = mousePoint.y - mousePointTopSpan;
		auto bottom = totalHeight - top - targetEleHeight;
		if (top < 0 || bottom < 50) {
			RemoveDocumentListener();
			return;
		}
		targetEle->SetProperty(Rml::PropertyId::Top, Rml::Property(top, Rml::Property::PX));
		targetEle->SetProperty(Rml::PropertyId::Bottom, Rml::Property(bottom, Rml::Property::PX));
		auto startTimeStr = PositionToTime(totalHeight, top);
		auto endTimeStr = PositionToTime(totalHeight, totalHeight - bottom);
		auto timeBox = targetEle->GetChild(1)->GetFirstChild();
		auto tag = timeBox->GetTagName();
		timeBox->GetFirstChild()->SetInnerRML(startTimeStr);
		timeBox->GetLastChild()->SetInnerRML(endTimeStr);

		//if (false) {
		//	auto xPoint = mousePoint.x - mousePointLeftSpan;
		//	targetEle->SetProperty(Rml::PropertyId::Left, Rml::Property(xPoint, Rml::Property::PX));
		//}
	}
	else if (dragType == 2) {  //拖动底部
		auto height = mousePoint.y - 50;
		auto bottom = totalHeight - height - 3;
		if (bottom < 0) {
			RemoveDocumentListener();
			return;
		}
		//不要让元素高度小于20
		if (totalHeight - targetEle->GetProperty(Rml::PropertyId::Top)->value.Get<float>() - bottom <= 20) {
			bottom = totalHeight - targetEle->GetProperty(Rml::PropertyId::Top)->value.Get<float>() - 20;
		}
		targetEle->SetProperty(Rml::PropertyId::Bottom, Rml::Property(bottom, Rml::Property::PX));
		auto endTimeStr = PositionToTime(totalHeight, totalHeight - bottom);
		targetEle->GetChild(1)->GetFirstChild()->GetLastChild()->SetInnerRML(endTimeStr);
	}
	else if (dragType == 3) { //拖动顶部
		auto top = mousePoint.y - 50 - 3;
		if (top < 0) {
			RemoveDocumentListener();
			return;
		}
		if (totalHeight - targetEle->GetProperty(Rml::PropertyId::Bottom)->value.Get<float>() - top <= 20) {
			top = totalHeight - targetEle->GetProperty(Rml::PropertyId::Bottom)->value.Get<float>() - 20;
		}
		targetEle->SetProperty(Rml::PropertyId::Top, Rml::Property(top, Rml::Property::PX));
		auto startTimeStr = PositionToTime(totalHeight, top);
		targetEle->GetChild(1)->GetFirstChild()->GetFirstChild()->SetInnerRML(startTimeStr);
	}
}

void ViewDay::ProcessEvent(Rml::Event& event) {
	auto ele = event.GetCurrentElement();
	auto eleId = ele->GetId();
	switch (event.GetId())
	{
		case Rml::EventId::Resize: {
			auto height = ele->GetClientHeight();			
			if (documentHeight != height) {
				if (documentHeight != 0) {
					//只有高度变化的时候再更新todoItem的位置

				}
				else
				{
					height = ele->GetParentNode()->GetClientHeight();
				}
				documentHeight = height;
			}			
		}
		case Rml::EventId::Mousedown: {
			targetEle = ele->GetParentNode();
			activeTodoEle(targetEle);
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
				RemoveDocumentListener();
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
