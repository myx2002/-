#include "record/schema.h"

/**
 * TODO: Student Implement
 */
uint32_t Schema::SerializeTo(char *buf) const {
  uint32_t ofs = 0;

  // MAGIC_NUM
  MACH_WRITE_UINT32(buf, SCHEMA_MAGIC_NUM);
  ofs += sizeof(uint32_t);

  // Column count
  MACH_WRITE_UINT32(buf + ofs, GetColumnCount());
  ofs += sizeof(uint32_t);

  for (uint32_t i = 0; i < columns_.size(); ++i) {
    ofs += columns_[i]->SerializeTo(buf + ofs);
  }

  // is_manage_
  MACH_WRITE_TO(bool, buf + ofs, is_manage_);
  ofs += sizeof(bool);
  buf += ofs;
  return ofs;
}

uint32_t Schema::GetSerializedSize() const {
  uint32_t ofs = sizeof(uint32_t); // Size for MAGIC_NUM field
  ofs += sizeof(uint32_t); // Size for column count field

  for (const auto& column : columns_) {
    ofs += column->GetSerializedSize();
  }

  ofs += sizeof(bool); // Size for is_manage_ field

  return ofs;
}

uint32_t Schema::DeserializeFrom(char *buf, Schema *&schema) {
  uint32_t ofs = 0;

  if (schema != nullptr) {
    LOG(WARNING) << "Pointer to schema is not null in schema deserialize." << std::endl;
  }

  // Deserialize MAGIC_NUM
  uint32_t magic_num = MACH_READ_UINT32(buf);
  ofs += sizeof(uint32_t);
  if (magic_num != SCHEMA_MAGIC_NUM) {
    // Handle error or return appropriate value indicating failure
    LOG(WARNING) << "SCHEMA_MAGIC_NUM doesn't match." << std::endl;
    return 0;
  }

  // Deserialize column count
  uint32_t column_count = MACH_READ_UINT32(buf + ofs);
  ofs += sizeof(uint32_t);

  // Deserialize columns
  std::vector<Column*> columns;
  for (uint32_t i = 0; i < column_count; ++i) {
    Column* column = nullptr;
    ofs += Column::DeserializeFrom(buf + ofs, column);
    columns.push_back(column);
  }

  // Deserialize is_manage_
  bool is_manage = MACH_READ_FROM(bool, buf + ofs);
  ofs += sizeof(bool);

  // Create schema object
  schema = new Schema(columns, is_manage);
  buf += ofs;
  return ofs;
}
