// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*- 
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2004-2006 Sage Weil <sage@newdream.net>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software 
 * Foundation.  See file COPYING.
 * 
 */

#ifndef CEPH_MOSDPGTRIM_H
#define CEPH_MOSDPGTRIM_H

#include "msg/Message.h"

class MOSDPGTrim : public Message {

  static const int HEAD_VERSION = 2;
  static const int COMPAT_VERSION = 2;

public:
  epoch_t epoch = 0;
  spg_t pgid;
  eversion_t trim_to;

  epoch_t get_epoch() const { return epoch; }

  MOSDPGTrim() : Message(MSG_OSD_PG_TRIM, HEAD_VERSION, COMPAT_VERSION) {}
  MOSDPGTrim(version_t mv, spg_t p, eversion_t tt) :
    Message(MSG_OSD_PG_TRIM, HEAD_VERSION, COMPAT_VERSION),
    epoch(mv), pgid(p), trim_to(tt) { }
private:
  ~MOSDPGTrim() override {}

public:
  const char *get_type_name() const override { return "pg_trim"; }
  void print(ostream& out) const override {
    out << "pg_trim(" << pgid << " to " << trim_to << " e" << epoch << ")";
  }

  void encode_payload(uint64_t features) override {
    ::encode(epoch, payload);
    ::encode(pgid.pgid, payload);
    ::encode(trim_to, payload);
    ::encode(pgid.shard, payload);
  }
  void decode_payload() override {
    bufferlist::iterator p = payload.begin();
    ::decode(epoch, p);
    ::decode(pgid.pgid, p);
    ::decode(trim_to, p);
    ::decode(pgid.shard, p);
  }
};

#endif
