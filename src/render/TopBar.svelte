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
  <div on:click={() => settingStore.changeView("Day")} class={`viewBtn ${curView === "Day" ? "viewBtnSelected" : ""}`}>日</div>
  <div on:click={() => settingStore.changeView("Week")} class={`viewBtn ${curView === "Week" ? "viewBtnSelected" : ""}`}>周</div>
  <div on:click={() => settingStore.changeView("Month")} class={`viewBtn ${curView === "Month" ? "viewBtnSelected" : ""}`}>月</div>
  <div on:click={() => settingStore.changeView("Year")} class={`viewBtn ${curView === "Year" ? "viewBtnSelected" : ""}`}>年</div>
  <div on:click={() => settingStore.changeView("List")} class={`viewBtn ${curView === "List" ? "viewBtnSelected" : ""}`}>
    <i style="font-size: 13px;" class="iconfont icon-sousuo" />
  </div>
  <div on:click={() => settingStore.changeView("Setting")} class={`viewBtn ${curView === "Setting" ? "viewBtnSelected" : ""}`}>
    <i class="iconfont icon-setting" />
  </div>
  <div class="span" />
</div>

<style lang="scss">
  .topBar {
    left: 0px;
    position: absolute;
    right: 0px;
    top: 0px;
    display: flex;
    height: 38px;
    line-height: 38px;
    font-size: 14px;
    background: #f1f3f4;
  }
  .winTitle {
    flex: 1;
    padding-left: 12px;
    font-size: 14px;
    -webkit-app-region: drag;
    border-bottom: 1px solid #dadce0;
  }
  .viewBtn {
    height: 34px;
    line-height: 30px;
    margin-top: 4px;
    width: 60px;
    text-align: center;
    cursor: pointer;
    border-top: 1px solid transparent;
    border-bottom: 1px solid #dadce0;
    box-sizing: border-box;
    &:hover {
      background: #f1f3f4;
    }
  }
  .viewBtnSelected {
    background: #ffffff;
    border-bottom: 1px solid #fff;
    border-left: 1px solid #dadce0;
    border-right: 1px solid #dadce0;
    border-top: 1px solid #dadce0;
    border-top-left-radius: 4px;
    border-top-right-radius: 4px;
    &:hover {
      background: #fff;
    }
  }
  .span {
    border-bottom: 1px solid #dadce0;
    width: 8px;
  }
</style>
