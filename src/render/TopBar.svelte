<script lang="ts">
  import type { SettingModel } from "../model/SettingModel";

  import { settingStore } from "./Store/SettingStore";
  let setting: SettingModel;
  let curView: string;
  settingStore.subscribe((v: SettingModel) => {
    setting = v;
    curView = setting.viewSelected;
    if (setting.viewSelected === "lastSelected") {
      curView = setting.viewLastSelected;
    }
  });
</script>

<div class="topBar">
  <div class="winTitle" />
  <div class="today">今日：2022-07-14</div>
  <div class={`viewBtn ${curView === "Day" ? "viewBtnSelected" : ""}`}>日</div>
  <div class={`viewBtn ${curView === "Week" ? "viewBtnSelected" : ""}`}>周</div>
  <div class={`viewBtn ${curView === "Month" ? "viewBtnSelected" : ""}`}>月</div>
  <div class={`viewBtn ${curView === "Year" ? "viewBtnSelected" : ""}`}>年</div>
  <div class="toolBtn">
    <i class="iconfont icon-sousuo" />
  </div>
  <div class="toolBtn">
    <i class="iconfont icon-setting" />
  </div>
</div>

<style lang="scss">
  .topBar {
    left: 0px;
    position: absolute;
    right: 0px;
    top: 0px;
    display: flex;
    height: 46px;
    line-height: 46px;
    font-size: 14px;
  }
  .winTitle {
    flex: 1;
    padding-left: 12px;
    font-size: 14px;
    -webkit-app-region: drag;
  }
  .today {
    padding-left: 12px;
    padding-right: 12px;
    cursor: pointer;
    &:hover {
      text-decoration: underline;
    }
  }
  .viewBtn {
    width: 38px;
    height: 24px;
    border: 1px solid #dadce0;
    line-height: 22px;
    margin-top: 10px;
    text-align: center;
    margin-left: 6px;
    margin-right: 6px;
    border-radius: 3px;
    cursor: pointer;
    &:hover {
      background: #f1f3f4;
    }
  }
  .viewBtnSelected {
    background: #f1f3f4;
  }
  .toolBtn {
    width: 40px;
    text-align: center;
    line-height: 46px;
    cursor: pointer;
  }
</style>
