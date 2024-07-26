/* SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause) */

/* THIS FILE IS AUTOGENERATED BY BPFTOOL! */
#ifndef __CS_DELAY_BPF_SKEL_H__
#define __CS_DELAY_BPF_SKEL_H__

#include <errno.h>
#include <stdlib.h>
#include <bpf/libbpf.h>

struct cs_delay_bpf {
	struct bpf_object_skeleton *skeleton;
	struct bpf_object *obj;
	struct {
		struct bpf_map *start;
		struct bpf_map *cs_ctrl_map;
		struct bpf_map *rb;
		struct bpf_map *rodata;
	} maps;
	struct {
		struct bpf_program *schedule;
		struct bpf_program *schedule_exit;
	} progs;
	struct {
		struct bpf_link *schedule;
		struct bpf_link *schedule_exit;
	} links;
	struct cs_delay_bpf__rodata {
		int ctrl_key;
	} *rodata;

#ifdef __cplusplus
	static inline struct cs_delay_bpf *open(const struct bpf_object_open_opts *opts = nullptr);
	static inline struct cs_delay_bpf *open_and_load();
	static inline int load(struct cs_delay_bpf *skel);
	static inline int attach(struct cs_delay_bpf *skel);
	static inline void detach(struct cs_delay_bpf *skel);
	static inline void destroy(struct cs_delay_bpf *skel);
	static inline const void *elf_bytes(size_t *sz);
#endif /* __cplusplus */
};

static void
cs_delay_bpf__destroy(struct cs_delay_bpf *obj)
{
	if (!obj)
		return;
	if (obj->skeleton)
		bpf_object__destroy_skeleton(obj->skeleton);
	free(obj);
}

static inline int
cs_delay_bpf__create_skeleton(struct cs_delay_bpf *obj);

static inline struct cs_delay_bpf *
cs_delay_bpf__open_opts(const struct bpf_object_open_opts *opts)
{
	struct cs_delay_bpf *obj;
	int err;

	obj = (struct cs_delay_bpf *)calloc(1, sizeof(*obj));
	if (!obj) {
		errno = ENOMEM;
		return NULL;
	}

	err = cs_delay_bpf__create_skeleton(obj);
	if (err)
		goto err_out;

	err = bpf_object__open_skeleton(obj->skeleton, opts);
	if (err)
		goto err_out;

	return obj;
err_out:
	cs_delay_bpf__destroy(obj);
	errno = -err;
	return NULL;
}

static inline struct cs_delay_bpf *
cs_delay_bpf__open(void)
{
	return cs_delay_bpf__open_opts(NULL);
}

static inline int
cs_delay_bpf__load(struct cs_delay_bpf *obj)
{
	return bpf_object__load_skeleton(obj->skeleton);
}

static inline struct cs_delay_bpf *
cs_delay_bpf__open_and_load(void)
{
	struct cs_delay_bpf *obj;
	int err;

	obj = cs_delay_bpf__open();
	if (!obj)
		return NULL;
	err = cs_delay_bpf__load(obj);
	if (err) {
		cs_delay_bpf__destroy(obj);
		errno = -err;
		return NULL;
	}
	return obj;
}

static inline int
cs_delay_bpf__attach(struct cs_delay_bpf *obj)
{
	return bpf_object__attach_skeleton(obj->skeleton);
}

static inline void
cs_delay_bpf__detach(struct cs_delay_bpf *obj)
{
	bpf_object__detach_skeleton(obj->skeleton);
}

static inline const void *cs_delay_bpf__elf_bytes(size_t *sz);

static inline int
cs_delay_bpf__create_skeleton(struct cs_delay_bpf *obj)
{
	struct bpf_object_skeleton *s;
	int err;

	s = (struct bpf_object_skeleton *)calloc(1, sizeof(*s));
	if (!s)	{
		err = -ENOMEM;
		goto err;
	}

	s->sz = sizeof(*s);
	s->name = "cs_delay_bpf";
	s->obj = &obj->obj;

	/* maps */
	s->map_cnt = 4;
	s->map_skel_sz = sizeof(*s->maps);
	s->maps = (struct bpf_map_skeleton *)calloc(s->map_cnt, s->map_skel_sz);
	if (!s->maps) {
		err = -ENOMEM;
		goto err;
	}

	s->maps[0].name = "start";
	s->maps[0].map = &obj->maps.start;

	s->maps[1].name = "cs_ctrl_map";
	s->maps[1].map = &obj->maps.cs_ctrl_map;

	s->maps[2].name = "rb";
	s->maps[2].map = &obj->maps.rb;

	s->maps[3].name = "cs_delay.rodata";
	s->maps[3].map = &obj->maps.rodata;
	s->maps[3].mmaped = (void **)&obj->rodata;

	/* programs */
	s->prog_cnt = 2;
	s->prog_skel_sz = sizeof(*s->progs);
	s->progs = (struct bpf_prog_skeleton *)calloc(s->prog_cnt, s->prog_skel_sz);
	if (!s->progs) {
		err = -ENOMEM;
		goto err;
	}

	s->progs[0].name = "schedule";
	s->progs[0].prog = &obj->progs.schedule;
	s->progs[0].link = &obj->links.schedule;

	s->progs[1].name = "schedule_exit";
	s->progs[1].prog = &obj->progs.schedule_exit;
	s->progs[1].link = &obj->links.schedule_exit;

	s->data = cs_delay_bpf__elf_bytes(&s->data_sz);

	obj->skeleton = s;
	return 0;
err:
	bpf_object__destroy_skeleton(s);
	return err;
}

static inline const void *cs_delay_bpf__elf_bytes(size_t *sz)
{
	static const char data[] __attribute__((__aligned__(8))) = "\
\x7f\x45\x4c\x46\x02\x01\x01\0\0\0\0\0\0\0\0\0\x01\0\xf7\0\x01\0\0\0\0\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\0\x10\x0e\0\0\0\0\0\0\0\0\0\0\x40\0\0\0\0\0\x40\0\x0c\0\
\x01\0\0\x2e\x73\x74\x72\x74\x61\x62\0\x2e\x73\x79\x6d\x74\x61\x62\0\x6b\x70\
\x72\x6f\x62\x65\x2f\x73\x63\x68\x65\x64\x75\x6c\x65\0\x6b\x72\x65\x74\x70\x72\
\x6f\x62\x65\x2f\x73\x63\x68\x65\x64\x75\x6c\x65\0\x6c\x69\x63\x65\x6e\x73\x65\
\0\x2e\x72\x6f\x64\x61\x74\x61\0\x2e\x6d\x61\x70\x73\0\x63\x73\x5f\x64\x65\x6c\
\x61\x79\x2e\x62\x70\x66\x2e\x63\0\x4c\x42\x42\x31\x5f\x33\0\x73\x63\x68\x65\
\x64\x75\x6c\x65\0\x63\x73\x5f\x63\x74\x72\x6c\x5f\x6d\x61\x70\0\x63\x74\x72\
\x6c\x5f\x6b\x65\x79\0\x73\x74\x61\x72\x74\0\x73\x63\x68\x65\x64\x75\x6c\x65\
\x5f\x65\x78\x69\x74\0\x72\x62\0\x4c\x49\x43\x45\x4e\x53\x45\0\x2e\x72\x65\x6c\
\x6b\x70\x72\x6f\x62\x65\x2f\x73\x63\x68\x65\x64\x75\x6c\x65\0\x2e\x72\x65\x6c\
\x6b\x72\x65\x74\x70\x72\x6f\x62\x65\x2f\x73\x63\x68\x65\x64\x75\x6c\x65\0\x2e\
\x42\x54\x46\0\x2e\x42\x54\x46\x2e\x65\x78\x74\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\0\x4a\0\0\0\x04\0\xf1\xff\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
\0\0\0\0\x03\0\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x03\0\x04\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\0\0\0\x59\0\0\0\0\0\x04\0\x18\x01\0\0\0\0\0\0\0\0\0\0\0\0\
\0\0\x60\0\0\0\x12\0\x03\0\0\0\0\0\0\0\0\0\xa0\0\0\0\0\0\0\0\x69\0\0\0\x11\0\
\x07\0\x20\0\0\0\0\0\0\0\x20\0\0\0\0\0\0\0\x75\0\0\0\x11\0\x06\0\0\0\0\0\0\0\0\
\0\x04\0\0\0\0\0\0\0\x7e\0\0\0\x11\0\x07\0\0\0\0\0\0\0\0\0\x20\0\0\0\0\0\0\0\
\x84\0\0\0\x12\0\x04\0\0\0\0\0\0\0\0\0\x28\x01\0\0\0\0\0\0\x92\0\0\0\x11\0\x07\
\0\x40\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\x95\0\0\0\x11\0\x05\0\0\0\0\0\0\0\0\0\
\x0d\0\0\0\0\0\0\0\x18\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x18\x02\0\0\0\0\0\0\0\0\
\0\0\0\0\0\0\x85\0\0\0\x01\0\0\0\x85\0\0\0\x05\0\0\0\x37\0\0\0\xe8\x03\0\0\x7b\
\x0a\xf8\xff\0\0\0\0\xb7\x01\0\0\0\0\0\0\x63\x1a\xf4\xff\0\0\0\0\xbf\xa2\0\0\0\
\0\0\0\x07\x02\0\0\xf4\xff\xff\xff\xbf\xa3\0\0\0\0\0\0\x07\x03\0\0\xf8\xff\xff\
\xff\x18\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xb7\x04\0\0\0\0\0\0\x85\0\0\0\x02\0\0\
\0\xb7\0\0\0\0\0\0\0\x95\0\0\0\0\0\0\0\x18\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x18\
\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x85\0\0\0\x01\0\0\0\x85\0\0\0\x05\0\0\0\xbf\
\x06\0\0\0\0\0\0\xb7\x01\0\0\0\0\0\0\x63\x1a\xfc\xff\0\0\0\0\xbf\xa2\0\0\0\0\0\
\0\x07\x02\0\0\xfc\xff\xff\xff\x18\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x85\0\0\0\
\x01\0\0\0\x15\0\x14\0\0\0\0\0\x79\x07\0\0\0\0\0\0\xbf\xa2\0\0\0\0\0\0\x07\x02\
\0\0\xfc\xff\xff\xff\x18\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x85\0\0\0\x03\0\0\0\
\x18\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xb7\x02\0\0\x18\0\0\0\xb7\x03\0\0\0\0\0\0\
\x85\0\0\0\x83\0\0\0\x15\0\x08\0\0\0\0\0\x37\x06\0\0\xe8\x03\0\0\x7b\x60\x08\0\
\0\0\0\0\x7b\x70\0\0\0\0\0\0\x1f\x76\0\0\0\0\0\0\x7b\x60\x10\0\0\0\0\0\xbf\x01\
\0\0\0\0\0\0\xb7\x02\0\0\0\0\0\0\x85\0\0\0\x84\0\0\0\xb7\0\0\0\0\0\0\0\x95\0\0\
\0\0\0\0\0\x44\x75\x61\x6c\x20\x42\x53\x44\x2f\x47\x50\x4c\0\0\0\0\0\0\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\0\0\x01\0\0\0\x06\0\0\0\x10\0\0\0\0\0\0\0\x01\0\0\0\x07\0\
\0\0\x70\0\0\0\0\0\0\0\x01\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\x01\0\0\0\x06\0\0\0\
\x10\0\0\0\0\0\0\0\x01\0\0\0\x07\0\0\0\x58\0\0\0\0\0\0\0\x01\0\0\0\x08\0\0\0\
\x90\0\0\0\0\0\0\0\x01\0\0\0\x08\0\0\0\xa8\0\0\0\0\0\0\0\x01\0\0\0\x0a\0\0\0\
\x9f\xeb\x01\0\x18\0\0\0\0\0\0\0\x8c\x03\0\0\x8c\x03\0\0\x89\x03\0\0\0\0\0\0\0\
\0\0\x02\x03\0\0\0\x01\0\0\0\0\0\0\x01\x04\0\0\0\x20\0\0\x01\0\0\0\0\0\0\0\x03\
\0\0\0\0\x02\0\0\0\x04\0\0\0\x02\0\0\0\x05\0\0\0\0\0\0\x01\x04\0\0\0\x20\0\0\0\
\0\0\0\0\0\0\0\x02\x06\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\0\x02\0\0\0\x04\0\0\0\x04\
\0\0\0\0\0\0\0\0\0\0\x02\x08\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\0\x02\0\0\0\x04\0\0\
\0\x08\0\0\0\0\0\0\0\0\0\0\x02\x0a\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\0\x02\0\0\0\
\x04\0\0\0\x01\0\0\0\0\0\0\0\x04\0\0\x04\x20\0\0\0\x19\0\0\0\x01\0\0\0\0\0\0\0\
\x1e\0\0\0\x05\0\0\0\x40\0\0\0\x27\0\0\0\x07\0\0\0\x80\0\0\0\x32\0\0\0\x09\0\0\
\0\xc0\0\0\0\x3e\0\0\0\0\0\0\x0e\x0b\0\0\0\x01\0\0\0\x44\0\0\0\0\0\0\x0e\x0b\0\
\0\0\x01\0\0\0\0\0\0\0\0\0\0\x02\x0f\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\0\x02\0\0\0\
\x04\0\0\0\x1b\0\0\0\0\0\0\0\0\0\0\x02\x11\0\0\0\0\0\0\0\0\0\0\x03\0\0\0\0\x02\
\0\0\0\x04\0\0\0\0\0\x04\0\0\0\0\0\x02\0\0\x04\x10\0\0\0\x19\0\0\0\x0e\0\0\0\0\
\0\0\0\x32\0\0\0\x10\0\0\0\x40\0\0\0\x50\0\0\0\0\0\0\x0e\x12\0\0\0\x01\0\0\0\0\
\0\0\0\0\0\0\x02\x15\0\0\0\x53\0\0\0\x15\0\0\x04\xa8\0\0\0\x5b\0\0\0\x16\0\0\0\
\0\0\0\0\x5f\0\0\0\x16\0\0\0\x40\0\0\0\x63\0\0\0\x16\0\0\0\x80\0\0\0\x67\0\0\0\
\x16\0\0\0\xc0\0\0\0\x6b\0\0\0\x16\0\0\0\0\x01\0\0\x6e\0\0\0\x16\0\0\0\x40\x01\
\0\0\x71\0\0\0\x16\0\0\0\x80\x01\0\0\x75\0\0\0\x16\0\0\0\xc0\x01\0\0\x79\0\0\0\
\x16\0\0\0\0\x02\0\0\x7c\0\0\0\x16\0\0\0\x40\x02\0\0\x7f\0\0\0\x16\0\0\0\x80\
\x02\0\0\x82\0\0\0\x16\0\0\0\xc0\x02\0\0\x85\0\0\0\x16\0\0\0\0\x03\0\0\x88\0\0\
\0\x16\0\0\0\x40\x03\0\0\x8b\0\0\0\x16\0\0\0\x80\x03\0\0\x8e\0\0\0\x16\0\0\0\
\xc0\x03\0\0\x96\0\0\0\x16\0\0\0\0\x04\0\0\x99\0\0\0\x16\0\0\0\x40\x04\0\0\x9c\
\0\0\0\x16\0\0\0\x80\x04\0\0\xa2\0\0\0\x16\0\0\0\xc0\x04\0\0\xa5\0\0\0\x16\0\0\
\0\0\x05\0\0\xa8\0\0\0\0\0\0\x01\x08\0\0\0\x40\0\0\0\0\0\0\0\x01\0\0\x0d\x02\0\
\0\0\xb6\0\0\0\x14\0\0\0\xba\0\0\0\x01\0\0\x0c\x17\0\0\0\xc3\0\0\0\x01\0\0\x0c\
\x17\0\0\0\xd1\0\0\0\0\0\0\x01\x01\0\0\0\x08\0\0\x01\0\0\0\0\0\0\0\x03\0\0\0\0\
\x1a\0\0\0\x04\0\0\0\x0d\0\0\0\xd6\0\0\0\0\0\0\x0e\x1b\0\0\0\x01\0\0\0\0\0\0\0\
\0\0\0\x0a\x02\0\0\0\xde\0\0\0\0\0\0\x0e\x1d\0\0\0\x01\0\0\0\x50\x03\0\0\x01\0\
\0\x0f\x0d\0\0\0\x1c\0\0\0\0\0\0\0\x0d\0\0\0\x58\x03\0\0\x01\0\0\x0f\x04\0\0\0\
\x1e\0\0\0\0\0\0\0\x04\0\0\0\x60\x03\0\0\x03\0\0\x0f\x50\0\0\0\x0c\0\0\0\0\0\0\
\0\x20\0\0\0\x0d\0\0\0\x20\0\0\0\x20\0\0\0\x13\0\0\0\x40\0\0\0\x10\0\0\0\0\x69\
\x6e\x74\0\x5f\x5f\x41\x52\x52\x41\x59\x5f\x53\x49\x5a\x45\x5f\x54\x59\x50\x45\
\x5f\x5f\0\x74\x79\x70\x65\0\x6b\x65\x79\x5f\x73\x69\x7a\x65\0\x76\x61\x6c\x75\
\x65\x5f\x73\x69\x7a\x65\0\x6d\x61\x78\x5f\x65\x6e\x74\x72\x69\x65\x73\0\x73\
\x74\x61\x72\x74\0\x63\x73\x5f\x63\x74\x72\x6c\x5f\x6d\x61\x70\0\x72\x62\0\x70\
\x74\x5f\x72\x65\x67\x73\0\x72\x31\x35\0\x72\x31\x34\0\x72\x31\x33\0\x72\x31\
\x32\0\x62\x70\0\x62\x78\0\x72\x31\x31\0\x72\x31\x30\0\x72\x39\0\x72\x38\0\x61\
\x78\0\x63\x78\0\x64\x78\0\x73\x69\0\x64\x69\0\x6f\x72\x69\x67\x5f\x61\x78\0\
\x69\x70\0\x63\x73\0\x66\x6c\x61\x67\x73\0\x73\x70\0\x73\x73\0\x75\x6e\x73\x69\
\x67\x6e\x65\x64\x20\x6c\x6f\x6e\x67\0\x63\x74\x78\0\x73\x63\x68\x65\x64\x75\
\x6c\x65\0\x73\x63\x68\x65\x64\x75\x6c\x65\x5f\x65\x78\x69\x74\0\x63\x68\x61\
\x72\0\x4c\x49\x43\x45\x4e\x53\x45\0\x63\x74\x72\x6c\x5f\x6b\x65\x79\0\x2f\x68\
\x6f\x6d\x65\x2f\x78\x68\x62\x2f\x6c\x6d\x70\x32\x2f\x6c\x6d\x70\x2f\x65\x42\
\x50\x46\x5f\x53\x75\x70\x65\x72\x6d\x61\x72\x6b\x65\x74\x2f\x43\x50\x55\x5f\
\x53\x75\x62\x73\x79\x73\x74\x65\x6d\x2f\x63\x70\x75\x5f\x77\x61\x74\x63\x68\
\x65\x72\x2f\x62\x70\x66\x2f\x63\x73\x5f\x64\x65\x6c\x61\x79\x2e\x62\x70\x66\
\x2e\x63\0\x20\x20\x20\x20\x63\x73\x5f\x63\x74\x72\x6c\x20\x3d\x20\x62\x70\x66\
\x5f\x6d\x61\x70\x5f\x6c\x6f\x6f\x6b\x75\x70\x5f\x65\x6c\x65\x6d\x28\x26\x63\
\x73\x5f\x63\x74\x72\x6c\x5f\x6d\x61\x70\x2c\x20\x26\x63\x74\x72\x6c\x5f\x6b\
\x65\x79\x29\x3b\0\x09\x74\x31\x20\x3d\x20\x62\x70\x66\x5f\x6b\x74\x69\x6d\x65\
\x5f\x67\x65\x74\x5f\x6e\x73\x28\x29\x2f\x31\x30\x30\x30\x3b\0\x09\x69\x6e\x74\
\x20\x6b\x65\x79\x20\x3d\x30\x3b\0\x09\x62\x70\x66\x5f\x6d\x61\x70\x5f\x75\x70\
\x64\x61\x74\x65\x5f\x65\x6c\x65\x6d\x28\x26\x73\x74\x61\x72\x74\x2c\x26\x6b\
\x65\x79\x2c\x26\x74\x31\x2c\x42\x50\x46\x5f\x41\x4e\x59\x29\x3b\0\x69\x6e\x74\
\x20\x42\x50\x46\x5f\x4b\x50\x52\x4f\x42\x45\x28\x73\x63\x68\x65\x64\x75\x6c\
\x65\x29\0\x09\x75\x36\x34\x20\x74\x32\x20\x3d\x20\x62\x70\x66\x5f\x6b\x74\x69\
\x6d\x65\x5f\x67\x65\x74\x5f\x6e\x73\x28\x29\x2f\x31\x30\x30\x30\x3b\0\x09\x69\
\x6e\x74\x20\x6b\x65\x79\x20\x3d\x20\x30\x3b\0\x09\x75\x36\x34\x20\x2a\x76\x61\
\x6c\x20\x3d\x20\x62\x70\x66\x5f\x6d\x61\x70\x5f\x6c\x6f\x6f\x6b\x75\x70\x5f\
\x65\x6c\x65\x6d\x28\x26\x73\x74\x61\x72\x74\x2c\x26\x6b\x65\x79\x29\x3b\0\x09\
\x69\x66\x20\x28\x76\x61\x6c\x20\x21\x3d\x20\x30\x29\x20\0\x20\x20\x20\x20\x20\
\x20\x20\x20\x09\x74\x31\x20\x3d\x20\x2a\x76\x61\x6c\x3b\0\x09\x09\x09\x62\x70\
\x66\x5f\x6d\x61\x70\x5f\x64\x65\x6c\x65\x74\x65\x5f\x65\x6c\x65\x6d\x28\x26\
\x73\x74\x61\x72\x74\x2c\x20\x26\x6b\x65\x79\x29\x3b\0\x09\x65\x20\x3d\x20\x62\
\x70\x66\x5f\x72\x69\x6e\x67\x62\x75\x66\x5f\x72\x65\x73\x65\x72\x76\x65\x28\
\x26\x72\x62\x2c\x20\x73\x69\x7a\x65\x6f\x66\x28\x2a\x65\x29\x2c\x20\x30\x29\
\x3b\0\x09\x69\x66\x20\x28\x21\x65\x29\x09\x72\x65\x74\x75\x72\x6e\x20\x30\x3b\
\x09\0\x09\x65\x2d\x3e\x74\x32\x3d\x74\x32\x3b\0\x09\x65\x2d\x3e\x74\x31\x3d\
\x74\x31\x3b\0\x20\x20\x20\x20\x20\x20\x20\x20\x09\x64\x65\x6c\x61\x79\x20\x3d\
\x20\x74\x32\x20\x2d\x20\x74\x31\x3b\0\x09\x65\x2d\x3e\x64\x65\x6c\x61\x79\x3d\
\x64\x65\x6c\x61\x79\x3b\0\x09\x62\x70\x66\x5f\x72\x69\x6e\x67\x62\x75\x66\x5f\
\x73\x75\x62\x6d\x69\x74\x28\x65\x2c\x20\x30\x29\x3b\0\x69\x6e\x74\x20\x42\x50\
\x46\x5f\x4b\x52\x45\x54\x50\x52\x4f\x42\x45\x28\x73\x63\x68\x65\x64\x75\x6c\
\x65\x5f\x65\x78\x69\x74\x29\0\x6c\x69\x63\x65\x6e\x73\x65\0\x2e\x72\x6f\x64\
\x61\x74\x61\0\x2e\x6d\x61\x70\x73\0\x6b\x70\x72\x6f\x62\x65\x2f\x73\x63\x68\
\x65\x64\x75\x6c\x65\0\x6b\x72\x65\x74\x70\x72\x6f\x62\x65\x2f\x73\x63\x68\x65\
\x64\x75\x6c\x65\0\0\0\0\x9f\xeb\x01\0\x20\0\0\0\0\0\0\0\x24\0\0\0\x24\0\0\0\
\xb4\x01\0\0\xd8\x01\0\0\0\0\0\0\x08\0\0\0\x66\x03\0\0\x01\0\0\0\0\0\0\0\x18\0\
\0\0\x76\x03\0\0\x01\0\0\0\0\0\0\0\x19\0\0\0\x10\0\0\0\x66\x03\0\0\x08\0\0\0\0\
\0\0\0\xe7\0\0\0\x38\x01\0\0\x0f\x8c\0\0\x28\0\0\0\xe7\0\0\0\x74\x01\0\0\x07\
\xbc\0\0\x30\0\0\0\xe7\0\0\0\x74\x01\0\0\x19\xbc\0\0\x38\0\0\0\xe7\0\0\0\x74\
\x01\0\0\x05\xbc\0\0\x48\0\0\0\xe7\0\0\0\x93\x01\0\0\x06\xc0\0\0\x58\0\0\0\xe7\
\0\0\0\0\0\0\0\0\0\0\0\x70\0\0\0\xe7\0\0\0\xa0\x01\0\0\x02\xc4\0\0\x90\0\0\0\
\xe7\0\0\0\xcf\x01\0\0\x05\xac\0\0\x76\x03\0\0\x12\0\0\0\0\0\0\0\xe7\0\0\0\x38\
\x01\0\0\x0f\x8c\0\0\x28\0\0\0\xe7\0\0\0\xe8\x01\0\0\x0b\xe4\0\0\x40\0\0\0\xe7\
\0\0\0\x0b\x02\0\0\x06\xec\0\0\x50\0\0\0\xe7\0\0\0\0\0\0\0\0\0\0\0\x58\0\0\0\
\xe7\0\0\0\x19\x02\0\0\x0d\xf0\0\0\x70\0\0\0\xe7\0\0\0\x47\x02\0\0\x06\xf4\0\0\
\x78\0\0\0\xe7\0\0\0\x57\x02\0\0\x0f\xfc\0\0\x88\0\0\0\xe7\0\0\0\0\0\0\0\0\0\0\
\0\x90\0\0\0\xe7\0\0\0\x6b\x02\0\0\x04\x04\x01\0\xa8\0\0\0\xe7\0\0\0\x91\x02\0\
\0\x06\x18\x01\0\xd0\0\0\0\xe7\0\0\0\xbf\x02\0\0\x06\x1c\x01\0\xd8\0\0\0\xe7\0\
\0\0\0\0\0\0\0\0\0\0\xe0\0\0\0\xe7\0\0\0\xd3\x02\0\0\x07\x24\x01\0\xe8\0\0\0\
\xe7\0\0\0\xde\x02\0\0\x07\x20\x01\0\xf0\0\0\0\xe7\0\0\0\xe9\x02\0\0\x15\0\x01\
\0\xf8\0\0\0\xe7\0\0\0\x03\x03\0\0\x0a\x28\x01\0\0\x01\0\0\xe7\0\0\0\x14\x03\0\
\0\x02\x2c\x01\0\x18\x01\0\0\xe7\0\0\0\x2f\x03\0\0\x05\xd8\0\0\0\0\0\0\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0\0\0\x03\0\0\0\x20\0\0\0\0\0\0\0\0\0\0\
\0\0\0\0\0\x40\0\0\0\0\0\0\0\xd6\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\0\
\0\0\0\0\0\0\0\0\0\x09\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x18\x01\
\0\0\0\0\0\0\x20\x01\0\0\0\0\0\0\x01\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\x18\0\0\0\
\0\0\0\0\x11\0\0\0\x01\0\0\0\x06\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x38\x02\0\0\0\0\
\0\0\xa0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x21\0\
\0\0\x01\0\0\0\x06\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xd8\x02\0\0\0\0\0\0\x28\x01\0\
\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x34\0\0\0\x01\0\0\
\0\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x04\0\0\0\0\0\0\x0d\0\0\0\0\0\0\0\0\0\0\
\0\0\0\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x3c\0\0\0\x01\0\0\0\x02\0\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\x10\x04\0\0\0\0\0\0\x04\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x04\
\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x44\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\0\0\0\0\0\
\0\0\0\x18\x04\0\0\0\0\0\0\x50\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\
\0\0\0\0\0\0\0\0\x9d\0\0\0\x09\0\0\0\x40\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x68\x04\
\0\0\0\0\0\0\x30\0\0\0\0\0\0\0\x02\0\0\0\x03\0\0\0\x08\0\0\0\0\0\0\0\x10\0\0\0\
\0\0\0\0\xb1\0\0\0\x09\0\0\0\x40\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x98\x04\0\0\0\0\
\0\0\x50\0\0\0\0\0\0\0\x02\0\0\0\x04\0\0\0\x08\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\
\xc8\0\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xe8\x04\0\0\0\0\0\0\x2d\
\x07\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xcd\0\0\0\
\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x18\x0c\0\0\0\0\0\0\xf8\x01\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

	*sz = sizeof(data) - 1;
	return (const void *)data;
}

#ifdef __cplusplus
struct cs_delay_bpf *cs_delay_bpf::open(const struct bpf_object_open_opts *opts) { return cs_delay_bpf__open_opts(opts); }
struct cs_delay_bpf *cs_delay_bpf::open_and_load() { return cs_delay_bpf__open_and_load(); }
int cs_delay_bpf::load(struct cs_delay_bpf *skel) { return cs_delay_bpf__load(skel); }
int cs_delay_bpf::attach(struct cs_delay_bpf *skel) { return cs_delay_bpf__attach(skel); }
void cs_delay_bpf::detach(struct cs_delay_bpf *skel) { cs_delay_bpf__detach(skel); }
void cs_delay_bpf::destroy(struct cs_delay_bpf *skel) { cs_delay_bpf__destroy(skel); }
const void *cs_delay_bpf::elf_bytes(size_t *sz) { return cs_delay_bpf__elf_bytes(sz); }
#endif /* __cplusplus */

__attribute__((unused)) static void
cs_delay_bpf__assert(struct cs_delay_bpf *s __attribute__((unused)))
{
#ifdef __cplusplus
#define _Static_assert static_assert
#endif
	_Static_assert(sizeof(s->rodata->ctrl_key) == 4, "unexpected size of 'ctrl_key'");
#ifdef __cplusplus
#undef _Static_assert
#endif
}

#endif /* __CS_DELAY_BPF_SKEL_H__ */
