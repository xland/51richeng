<script lang="ts">
  import { settingStore } from "../Store/SettingStore";
  export let viewName: string;
</script>

<div class="topBar">
  <div class="winTitle" />
  <div on:click={() => settingStore.changeView("Day")} class="viewBtn {`${viewName === 'Day' ? 'viewBtnSelected' : ''}`}">日</div>
  <div on:click={() => settingStore.changeView("Week")} class="viewBtn {`${viewName === 'Week' ? 'viewBtnSelected' : ''}`}">周</div>
  <div on:click={() => settingStore.changeView("Month")} class="viewBtn {`${viewName === 'Month' ? 'viewBtnSelected' : ''}`}">月</div>
  <div on:click={() => settingStore.changeView("Year")} class="viewBtn {`${viewName === 'Year' ? 'viewBtnSelected' : ''}`}">年</div>
  <div on:click={() => settingStore.changeView("List")} class="viewBtn {`${viewName === 'List' ? 'viewBtnSelected' : ''}`}">
    <i style="font-size: 13px;" class="iconfont icon-sousuo" />
  </div>
  <div on:click={() => settingStore.changeView("Setting")} class={`viewBtn ${viewName === "Setting" ? "viewBtnSelected" : ""}`}>
    <i class="iconfont icon-setting" />
  </div>
  <div class="span" />
</div>

<style lang="scss">
  .topBar {
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
