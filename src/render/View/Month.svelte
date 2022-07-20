<script lang="ts">
  import { onMount } from "svelte";
  import { lunar } from "../Common/lunar";
  type DateItem = { date: Date; isShowMonth: boolean; isCurDate: boolean; chineseDate: string };
  let dateList: DateItem[][] = [];

  //当前时间
  let curDate = new Date();
  let chineseName = "";
  let isCurDate = (d: Date) => {
    return curDate.getFullYear() === d.getFullYear() && curDate.getMonth() === d.getMonth() && curDate.getDate() === d.getDate();
  };

  //显示时间
  let showDate = new Date(curDate.getFullYear(), curDate.getMonth(), 1);
  let isShowMonth = (d: Date) => {
    return showDate.getFullYear() === d.getFullYear() && showDate.getMonth() === d.getMonth();
  };
  let goToMonth = (monthNum: number, year?: number) => {
    showDate.setMonth(monthNum);
    if (year != undefined) showDate.setFullYear(year);
    showDate = showDate;
    initDateList();
  };
  /**
   * 初始化一个月的数据列表
   */
  let initDateList = () => {
    dateList = [];
    let firstDay = new Date(showDate.getFullYear(), showDate.getMonth(), 1);
    let weekNum = firstDay.getDay();
    firstDay.setDate(0 - weekNum + 2);
    for (let row = 0; row < 6; row++) {
      let arr: DateItem[] = [];
      for (let i = 0; i < 7; i++) {
        let numArr = [firstDay.getFullYear(), firstDay.getMonth(), firstDay.getDate()];
        let lunarObj = lunar.Lunar.fromDate(firstDay);
        let item: DateItem = {
          date: new Date(numArr[0], numArr[1], numArr[2]),
          chineseDate: lunarObj.getDayInChinese(),
          isCurDate: isCurDate(firstDay),
          isShowMonth: isShowMonth(firstDay),
        };
        if (item.isCurDate) {
          chineseName = lunarObj.getMonthInChinese() + "月" + lunarObj.getDayInChinese();
        }
        firstDay.setDate(firstDay.getDate() + 1);
        arr.push(item);
      }
      dateList.push(arr);
    }
    dateList = dateList;
  };
  let addSchedule = async () => {
    let openWindow = (url): Promise<Window> => {
      return new Promise((resolve, reject) => {
        let dialog = window.open(url);
        dialog.window.parent = window;
        let dialogId = require("crypto").randomUUID();
        dialog.window["__dialogId"] = dialogId;
        let readyHandler = (e) => {
          let msg = JSON.parse(e.data);
          if (msg["msgName"] === dialogId) {
            window.removeEventListener("message", readyHandler);
            resolve(dialog);
          }
        };
        window.addEventListener("message", readyHandler);
      });
    };
    let dialog = await openWindow("http://localhost:3000/?dialog=Schedule");
    setInterval(() => {
      console.log("post");
      dialog.postMessage({
        name: "allen",
        value: { a: 123, b: "asdfasdfasd测试测试" },
      });
    }, 3000);
  };
  onMount(() => {
    initDateList();
  });
</script>

<div class="viewBox monthView">
  <div class="titleRow">
    <div class="monthTitle">{`${showDate.getFullYear()}年${showDate.getMonth() + 1}月`}</div>
    <div on:click={() => goToMonth(showDate.getMonth() - 1)} class="titleRowBtn">
      <i class="iconfont icon-zuojiantou" />
    </div>
    <div on:click={() => goToMonth(curDate.getMonth(), curDate.getFullYear())} class="titleRowBtn today">今天：{curDate.getFullYear()}-{curDate.getMonth() + 1}-{curDate.getDate()}（{chineseName}）</div>
    <div on:click={() => goToMonth(showDate.getMonth() + 1)} class="titleRowBtn">
      <i class="iconfont icon-youjiantou" />
    </div>
  </div>
  <div class="weekTitle">
    <div>一</div>
    <div>二</div>
    <div>三</div>
    <div>四</div>
    <div>五</div>
    <div>六</div>
    <div>日</div>
  </div>
  {#each dateList as arr, index (index)}
    <div class="dayRow">
      {#each arr as item (item.date)}
        <div on:click={addSchedule} class={item.isShowMonth ? "" : "notShowMonth"}>
          <div class={`dateTitle ${item.isCurDate ? "curDate" : ""}`}>{`${item.date.getDate()}（${item.chineseDate}）`}</div>
        </div>
      {/each}
    </div>
  {/each}
</div>

<style lang="scss">
  .monthView {
    display: flex;
    flex-direction: column;
  }
  .titleRow {
    display: flex;
    height: 42px;
    line-height: 42px;
    padding-left: 12px;
    padding-right: 8px;
    font-size: 12px;
    border-bottom: 1px solid #e1e2e3;
    .monthTitle {
      flex: 1;
    }
    .titleRowBtn {
      margin-top: 8px;
      height: 26px;
      width: 30px;
      line-height: 26px;
      text-align: center;
      border-radius: 3px;
      cursor: pointer;
      &:hover {
        background: #f1f2f3;
      }
      &:active {
        background: #e1e2e3;
      }
      i {
        font-size: 12px;
      }
    }
  }
  .today {
    width: auto !important;
    padding-left: 6px;
    padding-right: 0px;
  }
  .weekTitle {
    display: flex;
    border-bottom: 1px solid #e1e2e3;
    box-sizing: border-box;
    height: 28px;
    line-height: 28px;
    div {
      flex: 1;
      text-align: center;
      border-right: 1px solid #e1e2e3;
    }
  }
  .dayRow {
    display: flex;
    flex: 1;
    text-align: center;
    border-bottom: 1px solid #e1e2e3;
    box-sizing: border-box;
    div {
      flex: 1;
      border-right: 1px solid #e1e2e3;
      box-sizing: border-box;
      cursor: pointer;
      &:last-child {
        border-right: none;
      }
      &:hover {
        background: #f1f2f3;
      }
    }
    .notShowMonth {
      background: #f9f9f9;
      color: #999;
    }
    .dateTitle {
      margin-top: 4px;
      height: 24px;
      line-height: 24px;
      padding-left: 4px;
      padding-right: 0px;
    }
    .curDate {
      color: #fff;
      background: #1890ff;
      display: inline-block;
      border-radius: 3px;
      &:hover {
        color: #fff;
        background: #1890ff;
      }
    }
  }
</style>
