import { BaseModel } from "./BaseModel";

export class ScheduleModel extends BaseModel {
  title: string;
  startTime: number;
  endTime: number;
  fullDay: boolean;
  docFiles: string;
  isCycle: boolean;
  updateTime: number;
  createTime: number;
  finish: boolean;
}
