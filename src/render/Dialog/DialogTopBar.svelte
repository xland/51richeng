<script lang="ts">
  let close = () => {
    let { ipcRenderer } = require("electron");
    ipcRenderer.invoke("closeWindow");
  };
</script>

<div class="topBar">
  <div class="title">title</div>
  <div on:click={close} class="closeBtn">
    <i class="iconfont icon-close" />
  </div>
</div>

<style lang="scss">
  .topBar {
    height: 38px;
    display: flex;
    background: #f1f3f4;
    line-height: 38px;
  }
  .title {
    flex: 1;
    font-size: 14px;
    -webkit-app-region: drag;
  }
  .closeBtn {
    width: 40px;
    text-align: center;
    color: #888;
    i {
      font-size: 13px;
    }
    &:hover {
      background: rgb(233, 234, 235);
      color: #333;
    }
  }
</style>
