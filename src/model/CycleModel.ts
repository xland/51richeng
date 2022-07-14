import { BaseModel } from "./BaseModel";

export class ScheduleModel extends BaseModel {
  scheduleId: string;
  /**
   * 0无限循环，1按次数循环，2按结束日期循环
   */
  hasEnd: number;
  /**
   * 循环次数
   */
  cycleTime = -1;
  /**
   * 结束日期
   */
  endTime = -1;
  ignoreHoliday = true;
  /**
   * 每天、每月、每年
   */
  byType = "day";
  /**
   * 每月第几个星期几
   */
  byMonthValue1: number;
  byMonthValue2: number;
  /**
   * 每周星期几
   */
  byWeekValue: number;
  /**
   * 每年几月几日
   */
  byYearValue1: number;
  byYearValue2: number;
}
