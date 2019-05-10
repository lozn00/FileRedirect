//
// VirtualApp Native Project
//
#include "hookzz.h"
#  include <sys/system_properties.h>
#include "base/logging.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h> /* Definition of AT_* constants */
#include <sys/stat.h>
//#include <fb/include/fb/ALog.h>
//#include <Substrate/CydiaSubstrate.h>
#include "IOUniformer.h"
#include "SandboxFs.h"
#include "Path.h"
#include "SymbolFinder.h"
#include "AMyLog.h"
#include <syscall.h>

#ifndef __NR_link
#define __NR_link 9
#endif


#ifndef __NR_mknod
#define __NR_mknod 14
#endif

#ifndef __NR_rename
#define __NR_rename 38
#endif


#ifndef __NR_fstatat64
#define __NR_fstatat64 300
#endif


#ifndef __NR_unlinkat
#define __NR_unlinkat 301
#endif

#ifndef __NR_chmod
#define __NR_chmod 15
#endif


#ifndef __NR_chown
#define __NR_chown 182
#endif


#ifndef __NR_mkdir
#define __NR_mkdir 39
#endif

#ifndef __NR_rmdir
#define __NR_rmdir 40
#endif

#ifndef __NR_readlink
#define __NR_readlink 85
#endif


#ifndef __NR_open
#define __NR_open 5
#endif


#ifndef __NR_lchown
#define __NR_lchown 16
#endif


#ifndef __NR_unlink
#define __NR_unlink 10
#endif

#ifndef __NR_symlink
#define __NR_symlink 83
#endif

#ifndef __NR_utimes
#define __NR_utimes 271
#endif

#ifndef __NR_access
#define __NR_access 33
#endif


#ifndef __NR_renameat
#define __NR_renameat 302
#endif


#ifndef __NR_truncate64
#define __NR_truncate64 193
#endif


#ifndef __NR_linkat
#define __NR_linkat 303
#endif


#ifndef __NR_fstat64
#define __NR_fstat64 197
#endif


#ifndef __NR_statfs64
#define __NR_statfs64 268
#endif


#ifndef __NR_lstat64
#define __NR_lstat64 196
#endif

#ifndef __NR_stat64
#define __NR_stat64 195
#endif


bool iu_loaded = false;

void IOUniformer::init_env_before_all() {
    if (iu_loaded)
        return;
    char *api_level_chars = getenv("V_API_LEVEL");
    char *preview_api_level_chars = getenv("V_PREVIEW_API_LEVEL");
    if (api_level_chars) {
        LOGW("Enter init before all.");
        int api_level = atoi(api_level_chars);
        int preview_api_level;
        preview_api_level = atoi(preview_api_level_chars);
        char keep_env_name[25];
        char forbid_env_name[25];
        char replace_src_env_name[25];
        char replace_dst_env_name[25];
        int i = 0;
        while (true) {
            sprintf(keep_env_name, "V_KEEP_ITEM_%d", i);
            char *item = getenv(keep_env_name);
            if (!item) {
                break;
            }
            add_keep_item(item);
            i++;
        }
        i = 0;
        while (true) {
            sprintf(forbid_env_name, "V_FORBID_ITEM_%d", i);
            char *item = getenv(forbid_env_name);
            if (!item) {
                break;
            }
            add_forbidden_item(item);
            i++;
        }
        i = 0;
        while (true) {
            sprintf(replace_src_env_name, "V_REPLACE_ITEM_SRC_%d", i);
            char *item_src = getenv(replace_src_env_name);
            if (!item_src) {
                break;
            }
            sprintf(replace_dst_env_name, "V_REPLACE_ITEM_DST_%d", i);
            char *item_dst = getenv(replace_dst_env_name);
            add_replace_item(item_src, item_dst);
            i++;
        }
        startUniformer(getenv("V_SO_PATH"), api_level, preview_api_level);
        iu_loaded = true;
    }
}


static inline void
hook_function(void *handle, const char *symbol, void *new_func, void **old_func) {
    void *addr = dlsym(handle, symbol);
    if (addr == NULL) {
        return;
    }
    ZzReplace(addr, new_func, old_func);
}


void onSoLoaded(const char *name, void *handle);

void IOUniformer::redirect(const char *orig_path, const char *new_path) {
    add_replace_item(orig_path, new_path);
}

const char *IOUniformer::query(const char *orig_path) {
    int res;
    return relocate_path(orig_path, &res);
}

void IOUniformer::whitelist(const char *_path) {
    add_keep_item(_path);
}

void IOUniformer::forbid(const char *_path) {
    add_forbidden_item(_path);
}


const char *IOUniformer::reverse(const char *_path) {
    return reverse_relocate_path(_path);
}


__BEGIN_DECLS

#define FREE(ptr, org_ptr) { if ((void*) ptr != NULL && (void*) ptr != (void*) org_ptr) { free((void*) ptr); } }

// int faccessat(int dirfd, const char *pathname, int mode, int flags);
HOOK_DEF(int, faccessat, int dirfd, const char *pathname, int mode, int flags) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_faccessat, dirfd, redirect_path, mode, flags);
    FREE(redirect_path, pathname);
    return ret;
}


// int fchmodat(int dirfd, const char *pathname, mode_t mode, int flags);
HOOK_DEF(int, fchmodat, int dirfd, const char *pathname, mode_t mode, int flags) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_fchmodat, dirfd, redirect_path, mode, flags);
    FREE(redirect_path, pathname);
    return ret;
}

int (*old_faccessat)(int __dirfd, const char *__path, int __mode, int __flags);

int new_faccessat_(int __dirfd, const char *pathname, int __mode, int __flags) {
    LOGW("new_faccessat_", pathname, __mode);
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
//    int ret = syscall(__NR_faccessat, dirfd, redirect_path, __mode, __flags);
//    return ret;
    int ret = old_faccessat(__dirfd, redirect_path, __mode, __flags);
    FREE(redirect_path, pathname);
    return ret;
}

int (*old_fstatat64)(int __dir_fd, const char* __path, struct stat64* __buf, int __flags);

int new_fstatat64_(int __dir_fd, const char *__path, struct stat64 *__buf, int __flags){
  /*  int res;
    LOGE("fstatat64 exec ");
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_fstatat64, dirfd, redirect_path, buf, flags);
    FREE(redirect_path, pathname);*/


    int res;
    const char *redirect_path = relocate_path(__path, &res);
    LOGW("new_fstatat64_ redirect_path path: %s ,source path :%s ",redirect_path,__path);
    int ret = old_fstatat64(__dir_fd, redirect_path, __buf, __flags);
   FREE(redirect_path, __path);
    return ret;
}






// int fchmod(const char *pathname, mode_t mode);
HOOK_DEF(int, fchmod, const char *pathname, mode_t mode) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_chmod, redirect_path, mode);
//    int ret = syscall(SYS_chmod, redirect_path, mode);
    FREE(redirect_path, pathname);
    return ret;
}

/**
 * < 21 api才hook,无视
 * @param dirfd
 * @param pathname
 * @param buf
 * @param flags
 * @return
 */
// int fstatat(int dirfd, const char *pathname, struct stat *buf, int flags);
HOOK_DEF(int, fstatat, int dirfd, const char *pathname, struct stat *buf, int flags) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    LOGE("fstatat exec ");
    int ret = syscall(__NR_fstatat64, dirfd, redirect_path, buf, flags);
    FREE(redirect_path, pathname);
    return ret;
}

// int fstatat64(int dirfd, const char *pathname, struct stat *buf, int flags);
HOOK_DEF(int, fstatat64, int dirfd, const char *pathname, struct stat *buf, int flags) {
    int res;
    LOGE("fstatat64 exec ");
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_fstatat64, dirfd, redirect_path, buf, flags);
    FREE(redirect_path, pathname);
    return ret;
}


// int fstat(const char *pathname, struct stat *buf, int flags);
HOOK_DEF(int, fstat, const char *pathname, struct stat *buf) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_fstat64, redirect_path, buf);
    FREE(redirect_path, pathname);
    return ret;
}


// int mknodat(int dirfd, const char *pathname, mode_t mode, dev_t dev);
HOOK_DEF(int, mknodat, int dirfd, const char *pathname, mode_t mode, dev_t dev) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_mknodat, dirfd, redirect_path, mode, dev);
    FREE(redirect_path, pathname);
    return ret;
}
// int mknod(const char *pathname, mode_t mode, dev_t dev);
HOOK_DEF(int, mknod, const char *pathname, mode_t mode, dev_t dev) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_mknod, redirect_path, mode, dev);
    FREE(redirect_path, pathname);
    return ret;
}


// int utimensat(int dirfd, const char *pathname, const struct timespec times[2], int flags);
HOOK_DEF(int, utimensat, int dirfd, const char *pathname, const struct timespec times[2],
         int flags) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_utimensat, dirfd, redirect_path, times, flags);
    FREE(redirect_path, pathname);
    return ret;
}


// int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group, int flags);
HOOK_DEF(int, fchownat, int dirfd, const char *pathname, uid_t owner, gid_t group, int flags) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_fchownat, dirfd, redirect_path, owner, group, flags);
    FREE(redirect_path, pathname);
    return ret;
}

// int chroot(const char *pathname);
HOOK_DEF(int, chroot, const char *pathname) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_chroot, redirect_path);
    FREE(redirect_path, pathname);
    return ret;
}


// int renameat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath);
HOOK_DEF(int, renameat, int olddirfd, const char *oldpath, int newdirfd, const char *newpath) {
    int res_old;
    int res_new;
    const char *redirect_path_old = relocate_path(oldpath, &res_old);
    const char *redirect_path_new = relocate_path(newpath, &res_new);
    int ret = syscall(__NR_renameat, olddirfd, redirect_path_old, newdirfd, redirect_path_new);
    FREE(redirect_path_old, oldpath);
    FREE(redirect_path_new, newpath);
    return ret;
}
// int rename(const char *oldpath, const char *newpath);
HOOK_DEF(int, rename, const char *oldpath, const char *newpath) {
    int res_old;
    int res_new;
    const char *redirect_path_old = relocate_path(oldpath, &res_old);
    const char *redirect_path_new = relocate_path(newpath, &res_new);
    int ret = syscall(__NR_rename, redirect_path_old, redirect_path_new);
    FREE(redirect_path_old, oldpath);
    FREE(redirect_path_new, newpath);
    return ret;
}


// int unlinkat(int dirfd, const char *pathname, int flags);
HOOK_DEF(int, unlinkat, int dirfd, const char *pathname, int flags) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_unlinkat, dirfd, redirect_path, flags);
    FREE(redirect_path, pathname);
    return ret;
}
// int unlink(const char *pathname);
HOOK_DEF(int, unlink, const char *pathname) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_unlink, redirect_path);
    FREE(redirect_path, pathname);
    return ret;
}


// int symlinkat(const char *oldpath, int newdirfd, const char *newpath);
HOOK_DEF(int, symlinkat, const char *oldpath, int newdirfd, const char *newpath) {
    int res_old;
    int res_new;
    const char *redirect_path_old = relocate_path(oldpath, &res_old);
    const char *redirect_path_new = relocate_path(newpath, &res_new);
    int ret = syscall(__NR_symlinkat, redirect_path_old, newdirfd, redirect_path_new);
    FREE(redirect_path_old, oldpath);
    FREE(redirect_path_new, newpath);
    return ret;
}
// int symlink(const char *oldpath, const char *newpath);
HOOK_DEF(int, symlink, const char *oldpath, const char *newpath) {
    int res_old;
    int res_new;
    const char *redirect_path_old = relocate_path(oldpath, &res_old);
    const char *redirect_path_new = relocate_path(newpath, &res_new);
    int ret = syscall(__NR_symlink, redirect_path_old, redirect_path_new);
    FREE(redirect_path_old, oldpath);
    FREE(redirect_path_new, newpath);
    return ret;
}


// int linkat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, int flags);
HOOK_DEF(int, linkat, int olddirfd, const char *oldpath, int newdirfd, const char *newpath,
         int flags) {
    int res_old;
    int res_new;
    const char *redirect_path_old = relocate_path(oldpath, &res_old);
    const char *redirect_path_new = relocate_path(newpath, &res_new);
    int ret = syscall(__NR_linkat, olddirfd, redirect_path_old, newdirfd, redirect_path_new, flags);
    FREE(redirect_path_old, oldpath);
    FREE(redirect_path_new, newpath);
    return ret;
}
// int link(const char *oldpath, const char *newpath);
HOOK_DEF(int, link, const char *oldpath, const char *newpath) {
    int res_old;
    int res_new;
    const char *redirect_path_old = relocate_path(oldpath, &res_old);
    const char *redirect_path_new = relocate_path(newpath, &res_new);
    int ret = syscall(__NR_link, redirect_path_old, redirect_path_new);
    FREE(redirect_path_old, oldpath);
    FREE(redirect_path_new, newpath);
    return ret;
}


// int utimes(const char *filename, const struct timeval *tvp);
HOOK_DEF(int, utimes, const char *pathname, const struct timeval *tvp) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_utimes, redirect_path, tvp);
    FREE(redirect_path, pathname);
    return ret;
}


// int access(const char *pathname, int mode);
HOOK_DEF(int, access, const char *pathname, int mode) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_access, redirect_path, mode);
    FREE(redirect_path, pathname);
    return ret;
}


// int chmod(const char *path, mode_t mode);
HOOK_DEF(int, chmod, const char *pathname, mode_t mode) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_chmod, redirect_path, mode);
    FREE(redirect_path, pathname);
    return ret;
}


// int chown(const char *path, uid_t owner, gid_t group);
HOOK_DEF(int, chown, const char *pathname, uid_t owner, gid_t group) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_chown, redirect_path, owner, group);
    FREE(redirect_path, pathname);
    return ret;
}


// int lstat(const char *path, struct stat *buf);
HOOK_DEF(int, lstat, const char *pathname, struct stat *buf) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_lstat64, redirect_path, buf);
    FREE(redirect_path, pathname);
    return ret;
}


// int stat(const char *path, struct stat *buf);
HOOK_DEF(int, stat, const char *pathname, struct stat *buf) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_stat64, redirect_path, buf);
    FREE(redirect_path, pathname);
    return ret;
}


// int mkdirat(int dirfd, const char *pathname, mode_t mode);
HOOK_DEF(int, mkdirat, int dirfd, const char *pathname, mode_t mode) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_mkdirat, dirfd, redirect_path, mode);
    FREE(redirect_path, pathname);
    return ret;
}
// int mkdir(const char *pathname, mode_t mode);
HOOK_DEF(int, mkdir, const char *pathname, mode_t mode) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_mkdir, redirect_path, mode);
    FREE(redirect_path, pathname);
    return ret;
}


// int rmdir(const char *pathname);
HOOK_DEF(int, rmdir, const char *pathname) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_rmdir, redirect_path);
    FREE(redirect_path, pathname);
    return ret;
}


// int readlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz);
HOOK_DEF(int, readlinkat, int dirfd, const char *pathname, char *buf, size_t bufsiz) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_readlinkat, dirfd, redirect_path, buf, bufsiz);
    FREE(redirect_path, pathname);
    return ret;
}
// ssize_t readlink(const char *path, char *buf, size_t bufsiz);
HOOK_DEF(ssize_t, readlink, const char *pathname, char *buf, size_t bufsiz) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    ssize_t ret = syscall(__NR_readlink, redirect_path, buf, bufsiz);
    FREE(redirect_path, pathname);
    return ret;
}


// int __statfs64(const char *path, size_t size, struct statfs *stat);
HOOK_DEF(int, __statfs64, const char *pathname, size_t size, struct statfs *stat) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_statfs64, redirect_path, size, stat);
    FREE(redirect_path, pathname);
    return ret;
}


// int truncate(const char *path, off_t length);
HOOK_DEF(int, truncate, const char *pathname, off_t length) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_truncate, redirect_path, length);
    FREE(redirect_path, pathname);
    return ret;
}

#define RETURN_IF_FORBID if(res == FORBID) return -1;

// int truncate64(const char *pathname, off_t length);
HOOK_DEF(int, truncate64, const char *pathname, off_t length) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    RETURN_IF_FORBID
    int ret = syscall(__NR_truncate64, redirect_path, length);
    FREE(redirect_path, pathname);
    return ret;
}


// int chdir(const char *path);
HOOK_DEF(int, chdir, const char *pathname) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    RETURN_IF_FORBID
    int ret = syscall(__NR_chdir, redirect_path);
    FREE(redirect_path, pathname);
    return ret;
}


// int __getcwd(char *buf, size_t size);
HOOK_DEF(int, __getcwd, char *buf, size_t size) {
    int ret = syscall(__NR_getcwd, buf, size);
    if (!ret) {

    }
    return ret;
}


// int __openat(int fd, const char *pathname, int flags, int mode);
HOOK_DEF(int, __openat, int fd, const char *pathname, int flags, int mode) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_openat, fd, redirect_path, flags, mode);
    FREE(redirect_path, pathname);
    return ret;
}
// int __open(const char *pathname, int flags, int mode);
HOOK_DEF(int, __open, const char *pathname, int flags, int mode) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_open, redirect_path, flags, mode);
    FREE(redirect_path, pathname);
    return ret;
}

// int __statfs (__const char *__file, struct statfs *__buf);
HOOK_DEF(int, __statfs, __const char *__file, struct statfs *__buf) {
    int res;
    const char *redirect_path = relocate_path(__file, &res);
    int ret = syscall(__NR_statfs, redirect_path, __buf);
    FREE(redirect_path, __file);
    return ret;
}

// int lchown(const char *pathname, uid_t owner, gid_t group);
HOOK_DEF(int, lchown, const char *pathname, uid_t owner, gid_t group) {
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    int ret = syscall(__NR_lchown, redirect_path, owner, group);
    FREE(redirect_path, pathname);
    return ret;
}

int inline getArrayItemCount(char *const array[]) {
    int i;
    for (i = 0; array[i]; ++i);
    return i;
}


char **build_new_env(char *const envp[]) {
    char *provided_ld_preload = NULL;
    int provided_ld_preload_index = -1;
    int orig_envp_count = getArrayItemCount(envp);

    for (int i = 0; i < orig_envp_count; i++) {
        if (strstr(envp[i], "LD_PRELOAD")) {
            provided_ld_preload = envp[i];
            provided_ld_preload_index = i;
        }
    }
    char ld_preload[200];
    char *so_path = getenv("V_SO_PATH");
    if (provided_ld_preload) {
        sprintf(ld_preload, "LD_PRELOAD=%s:%s", so_path, provided_ld_preload + 11);
    } else {
        sprintf(ld_preload, "LD_PRELOAD=%s", so_path);
    }
    int new_envp_count = orig_envp_count
                         + get_keep_item_count()
                         + get_forbidden_item_count()
                         + get_replace_item_count() * 2 + 1;
    if (provided_ld_preload) {
        new_envp_count--;
    }
    char **new_envp = (char **) malloc(new_envp_count * sizeof(char *));
    int cur = 0;
    new_envp[cur++] = ld_preload;
    for (int i = 0; i < orig_envp_count; ++i) {
        if (i != provided_ld_preload_index) {
            new_envp[cur++] = envp[i];
        }
    }
    for (int i = 0; environ[i]; ++i) {
        if (environ[i][0] == 'V' && environ[i][1] == '_') {
            new_envp[cur++] = environ[i];
        }
    }
    new_envp[cur] = NULL;
    return new_envp;
}

// int (*origin_execve)(const char *pathname, char *const argv[], char *const envp[]);
HOOK_DEF(int, execve, const char *pathname, char *argv[], char *const envp[]) {
    /**
     * CANNOT LINK EXECUTABLE "/system/bin/cat": "/data/app/io.virtualapp-1/lib/arm/libva-native.so" is 32-bit instead of 64-bit.
     *
     * We will support 64Bit to adopt it.
     */
    // ALOGE("execve : %s", pathname); // any output can break exec. See bug: https://issuetracker.google.com/issues/109448553
    int res;
    const char *redirect_path = relocate_path(pathname, &res);
    char *ld = getenv("LD_PRELOAD");
    if (ld) {
        if (strstr(ld, "libNimsWrap.so") || strstr(ld, "stamina.so")) {
            int ret = syscall(__NR_execve, redirect_path, argv, envp);
            FREE(redirect_path, pathname);
            return ret;
        }
    }
    if (strstr(pathname, "dex2oat")) {
        char **new_envp = build_new_env(envp);
        int ret = syscall(__NR_execve, redirect_path, argv, new_envp);
        FREE(redirect_path, pathname);
        free(new_envp);
        return ret;
    }
    int ret = syscall(__NR_execve, redirect_path, argv, envp);
    FREE(redirect_path, pathname);
    return ret;
}


HOOK_DEF(void*, dlopen, const char *filename, int flag) {
    int res;
    const char *redirect_path = relocate_path(filename, &res);
    void *ret = orig_dlopen(redirect_path, flag);
    onSoLoaded(filename, ret);
    LOGW("dlopen : %s, return : %p.", redirect_path, ret);
    FREE(redirect_path, filename);
    return ret;
}

HOOK_DEF(void*, do_dlopen_V19, const char *filename, int flag, const void *extinfo) {
    int res;
    const char *redirect_path = relocate_path(filename, &res);
    void *ret = orig_do_dlopen_V19(redirect_path, flag, extinfo);
    onSoLoaded(filename, ret);
    LOGW("do_dlopen : %s, return : %p.", redirect_path, ret);
    FREE(redirect_path, filename);
    return ret;
}

HOOK_DEF(void*, do_dlopen_V24, const char *name, int flags, const void *extinfo,
         void *caller_addr) {
    int res;
    const char *redirect_path = relocate_path(name, &res);
    void *ret = orig_do_dlopen_V24(redirect_path, flags, extinfo, caller_addr);
    onSoLoaded(name, ret);
    LOGW("do_dlopen : %s, return : %p.", redirect_path, ret);
    FREE(redirect_path, name);
    return ret;
}



//void *dlsym(void *handle,const char *symbol)
HOOK_DEF(void*, dlsym, void *handle, char *symbol) {
    LOGW("dlsym : %p %s.", handle, symbol);
    return orig_dlsym(handle, symbol);
}

// int kill(pid_t pid, int sig);
HOOK_DEF(int, kill, pid_t pid, int sig) {
    LOGW(">>>>> kill >>> pid: %d, sig: %d.", pid, sig);
    int ret = syscall(__NR_kill, pid, sig);
    return ret;
}

HOOK_DEF(pid_t, vfork) {
    return fork();
}

__END_DECLS
// end IO DEF


void onSoLoaded(const char *name, void *handle) {
}


int findSymbol(const char *name, const char *libn,
               unsigned long *addr) {
    return find_name(getpid(), name, libn, addr);
}

void hook_dlopen(int api_level) {
    void *symbol = NULL;
    if (api_level > 23) {
        if (findSymbol("__dl__Z9do_dlopenPKciPK17android_dlextinfoPv", "linker",
                       (unsigned long *) &symbol) == 0) {
            ZzReplace(symbol, (void *) new_do_dlopen_V24,
                      (void **) &orig_do_dlopen_V24);
        }
    } else if (api_level >= 19) {
        if (findSymbol("__dl__Z9do_dlopenPKciPK17android_dlextinfo", "linker",
                       (unsigned long *) &symbol) == 0) {
            ZzReplace(symbol, (void *) new_do_dlopen_V19,
                      (void **) &orig_do_dlopen_V19);
        }
    } else {
        if (findSymbol("__dl_dlopen", "linker",
                       (unsigned long *) &symbol) == 0) {
            ZzReplace(symbol, (void *) new_dlopen, (void **) &orig_dlopen);
        }
    }
}


void IOUniformer::startUniformer(const char *so_path, int api_level, int preview_api_level) {
    char api_level_chars[5];
    setenv("V_SO_PATH", so_path, 1);
    sprintf(api_level_chars, "%i", api_level);

    setenv("V_API_LEVEL", api_level_chars, 1);
    sprintf(api_level_chars, "%i", preview_api_level);
    setenv("V_PREVIEW_API_LEVEL", api_level_chars, 1);

    void *handle = dlopen("libc.so", RTLD_NOW);
    char prop_value[PROP_VALUE_MAX];
    char prop_value_brand[PROP_VALUE_MAX];
    char prop_value_cpu[PROP_VALUE_MAX];
    //ro.product.brand]
    bool regiseterFaccessat = true;
    bool registerfstatat64 = true;
    if (handle) {
        //ro.vendor.product.cpu.abilist]
        //SM-
        __system_property_get("ro.build.version.sdk", prop_value);
        __system_property_get("ro.product.brand", prop_value_brand);
        __system_property_get("ro.vendor.product.cpu.abilist", prop_value_cpu);
        int api = atoi(prop_value);
        //
        if (api == 23) {
            memset(prop_value, 0, sizeof(prop_value));
            __system_property_get("ro.product.model", prop_value);
            if (strcmp(prop_value, "Redmi 4A") == 0) {
                regiseterFaccessat = false;
                LOGW("IGNORE_REGISTER_FUNC");
            }
        }
            std::string string_cpu = prop_value_cpu;
            if (string_cpu.find("arm64") >= 0) {
                LOGW(" ARM64 PHONE");
                registerfstatat64 = false;
            }

//        if (regiseterFaccessat) {
//            LOGW("faccessat_old");
//
//            HOOK_SYMBOL(handle, faccessat);
//        } else {
            hook_function(handle, "faccessat", (void *) new_faccessat_, (void **) &old_faccessat);
            LOGW("faccessat_ hook succ");
//        }

        LOGW("__openat");
        HOOK_SYMBOL(handle, __openat);
        HOOK_SYMBOL(handle, fchmodat);
        HOOK_SYMBOL(handle, fchownat);
        HOOK_SYMBOL(handle, renameat);


        LOGW("mkdirat");
        HOOK_SYMBOL(handle, mkdirat);
        //三星A70闪退?? fstatat64
//        Cause: seccomp prevented call to disallowed arm64 system call 300
//    if (registerfstatat64) {
//         HOOK_SYMBOL(handle, fstatat64);
//
//       } else {
            LOGW(" HOOK fstatat64");
    hook_function(handle, "fstatat64", (void *) new_fstatat64_, (void **) &old_fstatat64);

//    }

        HOOK_SYMBOL(handle, __statfs);
        HOOK_SYMBOL(handle, __statfs64);
        LOGW("mknodat");

        HOOK_SYMBOL(handle, mknodat);
        HOOK_SYMBOL(handle, truncate);
        HOOK_SYMBOL(handle, linkat);
        LOGW("readlinkat");
        HOOK_SYMBOL(handle, readlinkat);
        HOOK_SYMBOL(handle, unlinkat);
        HOOK_SYMBOL(handle, symlinkat);
        HOOK_SYMBOL(handle, utimensat);
        HOOK_SYMBOL(handle, __getcwd);

        LOGW("chdir");
        HOOK_SYMBOL(handle, chdir);
        LOGW("execve");
        HOOK_SYMBOL(handle, execve);
        LOGW("execve over");
        if (api_level <= 20) {
            HOOK_SYMBOL(handle, access);
            HOOK_SYMBOL(handle, __open);
            HOOK_SYMBOL(handle, stat);
            HOOK_SYMBOL(handle, lstat);
            HOOK_SYMBOL(handle, fstatat);
            HOOK_SYMBOL(handle, chmod);
            HOOK_SYMBOL(handle, chown);
            HOOK_SYMBOL(handle, rename);
            HOOK_SYMBOL(handle, rmdir);
            HOOK_SYMBOL(handle, mkdir);
            HOOK_SYMBOL(handle, mknod);
            HOOK_SYMBOL(handle, link);
            HOOK_SYMBOL(handle, unlink);
            HOOK_SYMBOL(handle, readlink);
            HOOK_SYMBOL(handle, symlink);
        }
        dlclose(handle);
    }
        hook_dlopen(api_level);
//
//    if (registerfstatat64) {
//        LOGW("dlopen start");
//        LOGW(" dlopen over");
//    }
}
