import { BaseModel } from "./BaseModel";
export class TagRelation extends BaseModel {
  tagId: string;
  relationId: string;
  relationType: string;
  createTime: number;
}
