class Eventer {
  private instance: any;
  emit(name: string, ...params) {
    this.instance.emit(name, ...params);
  }
  on(name: string, callback: (...args: any[]) => void) {
    this.instance.on(name, callback);
  }
  off(name: string, callback: (...args: any[]) => void) {
    this.instance.off(name, callback);
  }
  once(name: string, callback: (...args: any[]) => void) {
    this.instance.once(name, callback);
  }
  constructor() {
    let events = require("events");
    this.instance = new events.EventEmitter();
  }
}
export let eventer = new Eventer();
