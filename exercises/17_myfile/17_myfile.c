#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_elf_type(uint16_t e_type) {
  const char *type_str;
  switch (e_type) {
    case ET_NONE: type_str = "NONE (No file type)"; break;
    case ET_REL:  type_str = "REL (Relocatable file)"; break;
    case ET_EXEC: type_str = "EXEC (Executable file)"; break;
    case ET_DYN:  type_str = "DYN (Shared object file)"; break;
    case ET_CORE: type_str = "CORE (Core file)"; break;
    default:      type_str = "Unknown"; break;
  }
  printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int main(int argc, char *argv[]) {
  char filepath[2][256] = {
    "./17_myfile.o",
    "./17_myfile",
  };

  int fd;
  Elf64_Ehdr ehdr;

  // TODO: 在这里添加你的代码
  for (int i = 0; i < 2; i++) {
    fd = open(filepath[i], O_RDONLY);
    if (fd < 0) {
      fprintf(stderr, "无法打开文件: %s\n", filepath[i]);
      continue;
    }

    if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
      fprintf(stderr, "读取 ELF 头失败: %s\n", filepath[i]);
      close(fd);
      continue;
    }

    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
      fprintf(stderr, "不是有效的 ELF 文件: %s\n", filepath[i]);
      close(fd);
      continue;
    }

    printf("文件: %s\n", filepath[i]);
    print_elf_type(ehdr.e_type);
    close(fd);
  }
  
  return 0;
}
