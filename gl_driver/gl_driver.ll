; ModuleID = 'gl_driver.c'
source_filename = "gl_driver.c"
target datalayout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i32:32:32-f32:32:32-i64:32-f64:32-a:0:32-n32"
target triple = "sim"

; Function Attrs: noinline nounwind optnone
define dso_local void @gl_graph_init(i32 noundef %width, i32 noundef %height) #0 {
entry:
  %width.addr = alloca i32, align 4
  %height.addr = alloca i32, align 4
  %a = alloca i32, align 4
  store i32 %width, i32* %width.addr, align 4
  store i32 %height, i32* %height.addr, align 4
  %0 = load i32, i32* %width.addr, align 4
  %1 = load i32, i32* %height.addr, align 4
  %add = add i32 %0, %1
  store i32 %add, i32* %a, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define dso_local void @gl_set_pixel(i32 noundef %x, i32 noundef %y, i32 noundef %rgba) #0 {
entry:
  %x.addr = alloca i32, align 4
  %y.addr = alloca i32, align 4
  %rgba.addr = alloca i32, align 4
  %a = alloca i32, align 4
  store i32 %x, i32* %x.addr, align 4
  store i32 %y, i32* %y.addr, align 4
  store i32 %rgba, i32* %rgba.addr, align 4
  %0 = load i32, i32* %x.addr, align 4
  %1 = load i32, i32* %y.addr, align 4
  %add = add i32 %0, %1
  store i32 %add, i32* %a, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define dso_local i32 @gl_window_is_open() #0 {
entry:
  %res = alloca i32, align 4
  store i32 171, i32* %res, align 4
  %0 = load i32, i32* %res, align 4
  ret i32 %0
}

; Function Attrs: noinline nounwind optnone
define dso_local i64 @gl_get_msecs() #0 {
entry:
  %res = alloca i32, align 4
  store i32 171, i32* %res, align 4
  %0 = load i32, i32* %res, align 4
  %conv = sext i32 %0 to i64
  ret i64 %conv
}

; Function Attrs: noinline nounwind optnone
define dso_local void @gl_flush() #0 {
entry:
  %a = alloca i32, align 4
  store i32 171, i32* %a, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define dso_local i32 @gl_rand() #0 {
entry:
  %res = alloca i32, align 4
  store i32 171, i32* %res, align 4
  %0 = load i32, i32* %res, align 4
  ret i32 %0
}

; Function Attrs: noinline nounwind optnone
define dso_local void @gl_clear() #0 {
entry:
  %a = alloca i32, align 4
  store i32 171, i32* %a, align 4
  ret void
}

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 15.0.0 (git@github.com:uslsteen/llvm_practice.git 915e7699352394f449aff00b8a28975814f656f1)"}
