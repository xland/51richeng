import { BaseModel } from "./BaseModel";

export class SettingModel extends BaseModel {
  leftBoxWidth = 300;
  windowIsMaximized = true;
  windowWidth: number;
  windowHeight: number;
}
