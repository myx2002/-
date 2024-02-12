#include "record/row.h"

/**
 * TODO: Student Implement
 */
uint32_t Row::SerializeTo(char *buf, Schema *schema) const {

  ASSERT(schema != nullptr, "Invalid schema before serialize.");
  ASSERT(schema->GetColumnCount() == fields_.size(), "Fields size do not match schema's column size.");
  // replace with your code here

  uint32_t ofs = 0;
 // return GetSerializedSize(schema);
  // Serialize header
  uint32_t field_count = fields_.size();

  MACH_WRITE_UINT32(buf , field_count);
  ofs += sizeof(uint32_t);

  //return ofs;
  // Serialize null bitmap
  for(auto field : fields_) {
    MACH_WRITE_TO(bool, buf+ofs, field->IsNull());
    ofs += sizeof(bool);
  }

  // Serialize fields
  for (uint32_t i = 0; i < field_count; ++i) {
    Field *field = fields_[i];
    if(!field->IsNull()) {
      field->SerializeTo(buf+ofs);
      ofs += field->GetSerializedSize();
    }
  }
  buf += ofs;
  return ofs;

}

uint32_t Row::DeserializeFrom(char *buf, Schema *schema) {
  ASSERT(schema != nullptr, "Invalid schema before serialize.");
  ASSERT(fields_.empty(), "Non empty field in row.");
  // replace with your code here
  uint32_t ofs = 0;

  // Deserialize header
  uint32_t field_count = MACH_READ_UINT32(buf + ofs);
  ofs += sizeof(uint32_t);

  // Deserialize null bitmap
  bool null_bitmap[field_count];
  for (uint32_t i = 0; i < field_count; i++) {
    null_bitmap[i] = MACH_READ_FROM(bool, buf+ofs);
    ofs += sizeof(bool);
  }
  /*uint32_t null_bitmap_size = (field_count + 7) / 8; // Calculate size of null bitmap in bytes
  const char *null_bitmap = buf + ofs;
  ofs += null_bitmap_size;*/

  // Deserialize fields
  fields_.resize(field_count, nullptr); // Resize fields vector to match the field count

  for (uint32_t i = 0; i < field_count; ++i) {
    bool is_null = (null_bitmap[i / 8] & (1 << (i % 8))) != 0;

    // Deserialize field if it's not null
    if (is_null) {
      fields_[i] = nullptr; // Set field to null
    } else {
      // Deserialize non-null field
      TypeId type_id = schema->GetColumn(i)->GetType();
      uint32_t field_size = Type::GetInstance(type_id)->DeserializeFrom(buf + ofs, &fields_[i], is_null);
      ofs += field_size;
      }
  }
  buf += ofs;
  return GetSerializedSize(schema);;
}

uint32_t Row::GetSerializedSize(Schema *schema) const {
  ASSERT(schema != nullptr, "Invalid schema before serialize.");
 // std::cout<<schema->GetColumnCount();
  //std::cout<<fields_.size();
 // EXPECT_EQ(schema->GetColumnCount(),fields_.size());
  //ASSERT(schema->GetColumnCount() == fields_.size(), "Fields size do not match schema's column size.");
  // replace with your code here
  uint32_t ofs = sizeof(uint32_t); // Size for field count in header

  uint32_t field_count = fields_.size();

  uint32_t null_bitmap_size = fields_.size() * sizeof(bool); // Calculate size of null bitmap in bytes
  ofs += null_bitmap_size; // Size for null bitmap
  for (uint32_t i = 0; i < fields_.size(); ++i) {
      bool is_null = (fields_[i] == nullptr);
      TypeId type_id = schema->GetColumn(i)->GetType();
      ofs += Type::GetInstance(type_id)->GetSerializedSize(*fields_[i], is_null);
  }
  return ofs;
}

void Row::GetKeyFromRow(const Schema *schema, const Schema *key_schema, Row &key_row) {
  auto columns = key_schema->GetColumns();
  std::vector<Field> fields;
  uint32_t idx;
  for (auto column : columns) {
    schema->GetColumnIndex(column->GetName(), idx);
    fields.emplace_back(*this->GetField(idx));
  }
  key_row = Row(fields);
}
