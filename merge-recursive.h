#ifndef MERGE_RECURSIVE_H
#define MERGE_RECURSIVE_H

#include "unpack-trees.h"
#include "string-list.h"

struct merge_options {
	const char *ancestor;
	const char *branch1;
	const char *branch2;
	enum {
		MERGE_RECURSIVE_NORMAL = 0,
		MERGE_RECURSIVE_OURS,
		MERGE_RECURSIVE_THEIRS
	} recursive_variant;
	const char *subtree_shift;
	unsigned buffer_output; /* 1: output at end, 2: keep buffered */
	unsigned renormalize : 1;
	long xdl_opts;
	int verbosity;
	int detect_rename;
	int diff_rename_limit;
	int merge_rename_limit;
	int rename_score;
	int needed_rename_limit;
	int show_rename_progress;
	int call_depth;
	struct strbuf obuf;
	struct hashmap current_file_dir_set;
	struct string_list df_conflict_file_set;
	struct unpack_trees_options unpack_opts;
};

struct dir_rename_entry {
	struct hashmap_entry ent; /* must be the first member! */
	char *dir;
	unsigned non_unique_new_dir:1;
	struct strbuf new_dir;
	struct string_list possible_new_dirs;
};

struct collision_entry {
	struct hashmap_entry ent; /* must be the first member! */
	char *target_file;
	struct string_list source_files;
	unsigned reported_already:1;
};

/* merge_trees() but with recursive ancestor consolidation */
int merge_recursive(struct merge_options *o,
		    struct commit *h1,
		    struct commit *h2,
		    struct commit_list *ancestors,
		    struct commit **result);

/* rename-detecting three-way merge, no recursion */
int merge_trees(struct merge_options *o,
		struct tree *head,
		struct tree *merge,
		struct tree *common,
		struct tree **result);

/*
 * "git-merge-recursive" can be fed trees; wrap them into
 * virtual commits and call merge_recursive() proper.
 */
int merge_recursive_generic(struct merge_options *o,
			    const struct object_id *head,
			    const struct object_id *merge,
			    int num_ca,
			    const struct object_id **ca,
			    struct commit **result);

void init_merge_options(struct merge_options *o);
struct tree *write_tree_from_memory(struct merge_options *o);

int parse_merge_opt(struct merge_options *out, const char *s);

#endif
