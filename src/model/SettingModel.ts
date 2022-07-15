import { BaseModel } from "./BaseModel";

export class SettingModel extends BaseModel {
  windowX: number;
  windowY: number;
  windowWidth = 800;
  windowHeight = 600;
  /**
   * 默认选中的视图，如果是lastSelected，那么就用上次选中的视图
   */
  viewSelected = "Month";
  viewLastSelected = "Month";
  /**
   * 全天事件每隔多久提醒一次，单位分钟
   */
  dayScheduleAlertTimeSpan = 10;
  /**
   * 事件发生前多久开始提醒，单位分钟
   */
  scheduleAlertTimeBefore = 20;

  showIconInTaskbar = false;

  pinToDesktop = true;

  showLunar = true;
}
