#include "record/column.h"

#include "glog/logging.h"

Column::Column(std::string column_name, TypeId type, uint32_t index, bool nullable, bool unique)
    : name_(std::move(column_name)), type_(type), table_ind_(index), nullable_(nullable), unique_(unique) {
  ASSERT(type != TypeId::kTypeChar, "Wrong constructor for CHAR type.");
  switch (type) {
    case TypeId::kTypeInt:
      len_ = sizeof(int32_t);
      break;
    case TypeId::kTypeFloat:
      len_ = sizeof(float_t);
      break;
    default:
      ASSERT(false, "Unsupported column type.");
  }
}

Column::Column(std::string column_name, TypeId type, uint32_t length, uint32_t index, bool nullable, bool unique)
    : name_(std::move(column_name)),
      type_(type),
      len_(length),
      table_ind_(index),
      nullable_(nullable),
      unique_(unique) {
  ASSERT(type == TypeId::kTypeChar, "Wrong constructor for non-VARCHAR type.");
}

Column::Column(const Column *other)
    : name_(other->name_),
      type_(other->type_),
      len_(other->len_),
      table_ind_(other->table_ind_),
      nullable_(other->nullable_),
      unique_(other->unique_) {}

/**
* TODO: Student Implement
*/
uint32_t Column::SerializeTo(char *buf) const {
 /* uint32_t ofs = 0;

  // MAGIC_NUM
  MACH_WRITE_UINT32(buf+ofs, COLUMN_MAGIC_NUM);
  buf += sizeof(uint32_t);

  // name_
  MACH_WRITE_STRING(buf+ofs, name_);
  ofs += name_.length() + 1;

  // type(enum)
  MACH_WRITE_UINT32(buf+ofs, type_);
  ofs += sizeof(int);

  // len
  MACH_WRITE_UINT32(buf+ofs, len_);
  ofs += sizeof(int);

  // table_ind
  MACH_WRITE_UINT32(buf+ofs, table_ind_);
  ofs += sizeof(int);

  // nullable
  MACH_WRITE_TO(bool, buf+ofs, nullable_);
  ofs += sizeof(bool);

  // unique
  MACH_WRITE_TO(bool, buf+ofs, unique_);
  ofs += sizeof(bool);

  return ofs;*/
  // write magic num
  MACH_WRITE_UINT32(buf, COLUMN_MAGIC_NUM);
  buf += sizeof(uint32_t);
  // write name
  uint32_t len = name_.length();
  MACH_WRITE_UINT32(buf, len);
  buf += sizeof(uint32_t);
  MACH_WRITE_STRING(buf, name_);
  buf += sizeof(char) * len;
  // write type
  MACH_WRITE_TO(TypeId, buf, type_);
  buf += sizeof(TypeId);
  // write len
  MACH_WRITE_UINT32(buf, len_);
  buf += sizeof(uint32_t);
  // write table_ind
  MACH_WRITE_UINT32(buf, table_ind_);
  buf += sizeof(uint32_t);
  // write nullable
  MACH_WRITE_TO(bool, buf, nullable_);
  buf += sizeof(bool);
  // write unique
  MACH_WRITE_TO(bool, buf, unique_);
  buf += sizeof(bool);
  return GetSerializedSize();
}


/**
 * TODO: Student Implement
 */
uint32_t Column::GetSerializedSize() const {
  uint32_t ofs = sizeof(uint32_t); // Size for MAGIC_NUM field
  ofs += MACH_STR_SERIALIZED_SIZE(name_); // Size for name string
  ofs += sizeof(TypeId); // Size for type field
  ofs += sizeof(uint32_t); // Size for len field
  ofs += sizeof(uint32_t); // Size for table_ind field
  ofs += sizeof(bool); // Size for nullable field
  ofs += sizeof(bool); // Size for unique field
  return ofs;

}


/**
 * TODO: Student Implement
 */
uint32_t Column::DeserializeFrom(char *buf, Column *&column) {
  uint32_t ofs = 0;
  char *initbuf = buf;
  if (column != nullptr) {
    LOG(WARNING) << "Pointer to column is not null in column deserialize." << std::endl;
  }

  // Deserialize MAGIC_NUM
  uint32_t magic_num = MACH_READ_UINT32(buf);
  buf += sizeof(uint32_t);
  if (magic_num != COLUMN_MAGIC_NUM) {
    LOG(WARNING) << "The MAGIC_NUM in column doesn't match." << std::endl;
    return 0;
  }

  // Deserialize name_
  uint32_t len = MACH_READ_UINT32(buf);
  buf += sizeof(uint32_t);
  std::string name  = std::string(buf, len);;
  ofs += name.length() + 1;
  buf += sizeof(char) * len;
  // Deserialize type_
  TypeId type = static_cast<TypeId>(MACH_READ_UINT32(buf));
  buf += sizeof(TypeId);

  // Deserialize len_
  len = MACH_READ_UINT32(buf);
  buf += sizeof(uint32_t);

  // Deserialize table_ind_
  uint32_t table_ind = MACH_READ_UINT32(buf);
  buf += sizeof(uint32_t);

  // Deserialize nullable_
  bool nullable = MACH_READ_FROM(bool, buf);
  buf += sizeof(bool);

  // Deserialize unique_
  bool unique = MACH_READ_FROM(bool, buf);
  buf += sizeof(bool);

  // Create column object based on type
  if (type == kTypeChar) {
    column = new Column(name, type, len, table_ind, nullable, unique);
  } else {
    column = new Column(name, type, table_ind, nullable, unique);

  }

  return (buf - initbuf);
}

