import type { BrowserWindowConstructorOptions } from "electron";
import { ConfigWebPreferences } from "./ConfigWebPreferences";

export class ConfigWindow implements BrowserWindowConstructorOptions {
  width?: number;
  height?: number;
  maximizable = true;
  resizable = true;
  center = true;
  x?: number;
  y?: number;
  alwaysOnTop?: boolean;
  skipTaskbar?: boolean;
  frame = false;
  show = false;
  webPreferences = new ConfigWebPreferences();
  nativeWindowOpen = true;
  momodalable = false;
  modal = false;
  parent?: any;
  movable = true;
  thickFrame = true;
  hasShadow = false;
  opacity = 1;
  minHeight?: number;
  minWidth?: number;
}
