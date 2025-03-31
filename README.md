This program are designed to appear with your desktop. After you pressed the Win+D and other windows disappeared, it would still be there.
It\`s a TO-DO list now.
I don\`t know how to use user32 by c#, so I choose QT.

Haven`t done.

after done, additional:
1.drag: press certain area can reorder entries.

代码结构指路:
StickyNote下的StickyNote为桌面便签窗口,Monitor监听前台窗口变化, 当切换到桌面时发出信号, 随后Elevator负责将窗口移动到顶端

TodoList下的TodoList是主体,TodoEntry负责每一条记录,AddEntry添加新记录. 

ToolBar负责小工具,例如回滚,锁定