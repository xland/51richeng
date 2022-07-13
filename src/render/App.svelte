<script lang="ts">
  import "./assets/main.scss";
  import MainBody from "./MainBody.svelte";
  import LeftBox from "./LeftBox.svelte";
  import ContextMenu from "./ContextMenu.svelte";
  import TopBar from "./TopBar.svelte";
  import Splitter from "./Splitter.svelte";
</script>

<LeftBox />
<Splitter />
<MainBody />
<TopBar />
<ContextMenu />

<style lang="scss">
</style>
