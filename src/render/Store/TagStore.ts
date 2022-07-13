import { get, writable } from "svelte/store";
import { db } from "../../common/db";
import { TagModel } from "../../model/TagModel";
import { tagRelationStore } from "./TagRelationStore";
class TagStore {
  store = writable<TagModel[]>([]);
  subscribe = this.store.subscribe;
  async init(id: string, type: string) {
    let tags = await db.select("Tag.*").from("Tag").leftJoin("TagRelation", "Tag.id", "TagRelation.tagId").where("TagRelation.relationId", id).andWhere("TagRelation.relationType", type);
    this.store.set(tags);
  }
  async getTagByTitle(title: string) {
    let tag = await db("Tag").where({ title }).first();
    return tag;
  }
  async addExistTag(tag: TagModel, articleId: string, type: string) {
    tag.count += 1;
    await db("Tag").update({ count: tag.count }).where({ id: tag.id });
    await tagRelationStore.addRelation(tag.id, articleId, type);
    let tags = get(this.store);
    tags.push(tag);
    this.store.set(tags);
  }
  async addTag(tagTitle: string, articleId: string, type: string) {
    let tag = new TagModel();
    tag.title = tagTitle;
    tag.createTime = Date.now();
    tag.count = 1;
    await db("Tag").insert(tag);
    await tagRelationStore.addRelation(tag.id, articleId, type);
    let tags = get(this.store);
    tags.push(tag);
    this.store.set(tags);
    return tag;
  }
}
export let tagStore = new TagStore();
