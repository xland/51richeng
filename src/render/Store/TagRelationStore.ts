import { db } from "../../common/db";
import { TagRelation } from "../../model/TagRelation";
class TagRelationStore {
  async addRelation(tagId: string, relationId: string, relationType: string) {
    let relation = new TagRelation();
    relation.tagId = tagId;
    relation.relationType = relationType;
    relation.relationId = relationId;
    relation.createTime = Date.now();
    await db("TagRelation").insert(relation);
  }
}
export let tagRelationStore = new TagRelationStore();
