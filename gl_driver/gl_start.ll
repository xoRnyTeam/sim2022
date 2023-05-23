; ModuleID = 'gl_start.c'
source_filename = "gl_start.c"
target datalayout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i32:32:32-f32:32:32-i64:32-f64:32-a:0:32-n32"
target triple = "sim"

; Function Attrs: noinline nounwind optnone
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %width = alloca i32, align 4
  %heigth = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 524, i32* %width, align 4
  store i32 524, i32* %heigth, align 4
  call void @gl_graph_init(i32 noundef 524, i32 noundef 524)
  store i32 0, i32* %x, align 4
  store i32 0, i32* %y, align 4
  br label %while.cond

while.cond:                                       ; preds = %if.end4, %entry
  %call = call i32 bitcast (i32 (...)* @gl_window_is_open to i32 ()*)()
  %tobool = icmp ne i32 %call, 0
  br i1 %tobool, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %0 = load i32, i32* %x, align 4
  %1 = load i32, i32* %y, align 4
  call void @gl_set_pixel(i32 noundef %0, i32 noundef %1, i32 noundef -2147418113)
  call void bitcast (void (...)* @gl_flush to void ()*)()
  %2 = load i32, i32* %x, align 4
  %inc = add nsw i32 %2, 1
  store i32 %inc, i32* %x, align 4
  %3 = load i32, i32* %x, align 4
  %cmp = icmp eq i32 %3, 524
  br i1 %cmp, label %if.then, label %if.end4

if.then:                                          ; preds = %while.body
  store i32 0, i32* %x, align 4
  %4 = load i32, i32* %y, align 4
  %inc1 = add nsw i32 %4, 1
  store i32 %inc1, i32* %y, align 4
  %5 = load i32, i32* %y, align 4
  %cmp2 = icmp eq i32 %5, 524
  br i1 %cmp2, label %if.then3, label %if.end

if.then3:                                         ; preds = %if.then
  store i32 0, i32* %y, align 4
  br label %if.end

if.end:                                           ; preds = %if.then3, %if.then
  br label %if.end4

if.end4:                                          ; preds = %if.end, %while.body
  br label %while.cond, !llvm.loop !3

while.end:                                        ; preds = %while.cond
  ret i32 0
}

declare dso_local void @gl_graph_init(i32 noundef, i32 noundef) #1

declare dso_local i32 @gl_window_is_open(...) #1

declare dso_local void @gl_set_pixel(i32 noundef, i32 noundef, i32 noundef) #1

declare dso_local void @gl_flush(...) #1

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 15.0.0 (git@github.com:uslsteen/llvm_practice.git 915e7699352394f449aff00b8a28975814f656f1)"}
!3 = distinct !{!3, !4}
!4 = !{!"llvm.loop.mustprogress"}
