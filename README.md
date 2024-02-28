<h1>Multifunctional Differentiator by Ruzal</h1>
<h2>Как работает?</h2>
<p>На вход поступает выражение в виде дерева. <b>Например</b>:</p>

```
( . x . ) + ( . x . )
```
<p>Точка обозначает NULL. Таким образом строится дерево</p>

![image](https://github.com/SvetoCopy/Differentiator/assets/65361271/a6d86667-7461-4e14-9d7a-37a3054ecf68)

<p>Все операции и их деревья будут ниже</p>
<h3>Дифференцирование</h3>
<p>После чтения дерева, выполняются стандартные операции взятия производной. <b>Например</b>:</p>

![image](https://github.com/SvetoCopy/Differentiator/assets/65361271/42b2a221-4dea-4937-a24b-ebcb65ad8160)

<p>Функция diff() обозначает дифференцирование элементарных элементов (переменных или чисел)</p>
<h3>Оптимизация</h3>
<p>
При дифференцированнии сложных функций, дерево производной этой функции становится довольно объемной, поэтому
производятся оптимизации, упрощающие деревья.
</p>
<h4>Первый тип оптимизации</h4>

<p>Этот тип оптимизации работает для поддеревьев, <i>не содержащих переменных</i>. Это поддерево заменяется вычисленным значением. <b>Например</b>:</p>

![image](https://github.com/SvetoCopy/Differentiator/assets/65361271/c31e15aa-b665-44aa-8b21-2cb26ea3fc3d)

<h4>Второй тип оптимизации</h4>
<p>Этот тип оптимизации работает для определенных случаев (в т.ч. содержащих переменные). Полный список этих случаев перечислен в таблице:</p>

| Было | Стало |
|------|-------|
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/e1196126-79fe-49dc-b551-9078fbafe3f4" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/fd7e9edd-3c7f-4a25-8cd7-d3cf6393da19" width="200" height="185"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/f9bc833a-650e-472c-83b8-0d87f523edbb" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/e41faefc-0aef-4d7a-a8b7-ded67cd89740" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/639af879-baa6-4bdc-a1d7-4da900a88694" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/e41faefc-0aef-4d7a-a8b7-ded67cd89740" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/b32d4a0a-59fb-456e-af5d-d53b5085ff72" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/e41faefc-0aef-4d7a-a8b7-ded67cd89740" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/54f9bd90-51f9-4b19-9b5f-c66fae89534e" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/5ff7f64c-fd7f-4c79-9684-ca343b786569" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/0ebb9a62-f355-4bd1-be8a-2e443cefd2f1" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/8bd89e42-0916-4e50-ac24-281f26ba9814" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/d1ca9ce3-fb85-45ab-a0c6-3be97ad496e3" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/8bd89e42-0916-4e50-ac24-281f26ba9814" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/f03a827f-6d44-4880-ba02-5e94f540bc16" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/e41faefc-0aef-4d7a-a8b7-ded67cd89740" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/8bdd6747-d15e-4a2e-9d80-9760b75c97b8" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/e41faefc-0aef-4d7a-a8b7-ded67cd89740" width="200" height="178"> |

<h2>Как выглядит результат?</h2>
