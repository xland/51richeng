<script lang="ts">
  import { onMount } from "svelte";

  import DialogTopBar from "./DialogTopBar.svelte";
  onMount(() => {
    let { ipcRenderer } = require("electron");
    ipcRenderer.invoke("sizeWindow", {
      w: 600,
      h: 330,
      minW: 600,
      minH: 330,
    });
    let msg = { msgName: window["__dialogId"] };
    window.parent.postMessage(JSON.stringify(msg));
    window.addEventListener("message", function (e) {
      console.log(e);
    });
  });
</script>

<DialogTopBar />
<!-- <webview id="webview" src="https://www.oschina.net/" style="height: 100%;width:100%;" allowpopups /> -->

<div class="Schedule">
  <div class="row title">
    <div class="iconBox">
      <i class="iconfont icon-edit" />
    </div>
    <input class="inputBox" placeholder="请输入事务标题，Enter键输入多行内容" type="text" />
  </div>
  <div class="row">
    <div class="iconBox">
      <i class="iconfont icon-zuijin" style="font-size: 18px;" />
    </div>
    <div class="inputBox">7月12日 16时12分15秒</div>
  </div>
  <div class="row">
    <div class="iconBox">
      <i class="iconfont icon-xunhuan" />
    </div>
    <div class="inputBox">不重复</div>
  </div>
  <div class="row">
    <div class="iconBox">
      <i class="iconfont icon-zuijin" style="font-size: 18px;" />
    </div>
    <div class="inputBox">提前30分钟提醒</div>
  </div>
  <div class="btnRow">
    <div class="saveBtn">保存</div>
  </div>
</div>

<style lang="scss">
  .Schedule {
    box-sizing: border-box;
    padding-left: 26px;
    padding-right: 26px;
    padding-top: 16px;
    padding-bottom: 16px;
    font-size: 16px;
  }
  .row {
    height: 36px;
    line-height: 36px;
    display: flex;
    padding-top: 12px;
    .iconBox {
      width: 38px;
      text-align: center;
    }
    .inputBox {
      flex: 1;
      height: 36px;
      line-height: 36px;
    }
  }
  .btnRow {
    padding-top: 16px;
    padding-right: 16px;
  }
  .saveBtn {
    background: #1890ff;
    color: #fff;
    float: right;
    padding-left: 18px;
    padding-right: 18px;
    padding-top: 8px;
    padding-bottom: 8px;
    border-radius: 4px;
    cursor: pointer;
    font-size: 16px;
    &:hover {
      background: #096dd9;
    }
  }
  .title {
    input {
      border-bottom: 1px solid #ddd;
      font-size: 16px;
    }
  }
</style>
