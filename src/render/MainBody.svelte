<script lang="ts">
  import ArticleEditor from "./Article/ArticleEditor.svelte";
  import ArticleTag from "./Article/ArticleTag.svelte";
  import ArticleTitle from "./Article/ArticleTitle.svelte";
</script>

<div class="MainBody" style="left: 300px;">
  <ArticleTitle />
  <ArticleEditor />
  <ArticleTag />
</div>

<style lang="scss">
  .MainBody {
    position: absolute;
    top: 30px;
    right: 0px;
    bottom: 0px;
    z-index: 1;
    display: flex;
  }
  .ArticleContentBody {
    padding-top: 30px;
    flex: 1;
    background: #fff;
    display: flex;
    flex-direction: column;
    position: relative;
  }
</style>
