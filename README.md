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

<p>Функция diff() обозначает дифференцирование элементов выражений(переменных или чисел)</p>
<h3>Оптимизация</h3>
<p>
При дифференцированнии сложных функций, дерево производной этой функции становится довольно объемным, поэтому
производятся оптимизации, упрощающие деревья.
</p>
<h4>Первый тип оптимизации - свертка констант</h4>

<p>Этот тип оптимизации работает для поддеревьев, <i>не содержащих переменных</i>. Это поддерево заменяется вычисленным значением. <b>Например</b>:</p>

![image](https://github.com/SvetoCopy/Differentiator/assets/65361271/c31e15aa-b665-44aa-8b21-2cb26ea3fc3d)

<h4>Второй тип оптимизации - удаление нейтральных элементов</h4>
<p>Этот тип оптимизации работает для определенных случаев (в т.ч. содержащих переменные). Полный список этих случаев перечислен в таблице:</p>

| Было | Стало |
|------|-------|
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/0c110352-ccff-4eb0-8722-a0b9f4fda61c" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/8c2a302b-203f-4896-a18d-f48c0d3e1cea" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/df2dd2e1-6d4f-45ec-8944-fa3df2e5b654" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/921aad47-2dd8-488a-9609-929b0739b928" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/6456d0da-1cd8-4e91-86a5-6ad3df1c3dd6" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/605828db-188e-4922-b98a-ea0347e71e07" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/5216871d-0940-4140-a98e-52e03a8b35e0" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/605828db-188e-4922-b98a-ea0347e71e07" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/e0148626-df6e-46c7-9ce9-c4f605ed9435" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/8c2a302b-203f-4896-a18d-f48c0d3e1cea" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/835ab379-e930-41e6-bb60-1e1f9d98ef5c" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/0bddd955-8904-4372-855d-21758c7ea54f" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/9c61b412-5526-4313-ae81-f42eccfcd7c5" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/0bddd955-8904-4372-855d-21758c7ea54f" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/852d386f-363f-4f72-88d1-14216f8fe228" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/921aad47-2dd8-488a-9609-929b0739b928" width="200" height="178"> |
|<img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/51afd554-2f9c-4d66-9eb7-5a3ca68d1e04" width="200" height="178"> | <img src="https://github.com/SvetoCopy/Differentiator/assets/65361271/605828db-188e-4922-b98a-ea0347e71e07" width="200" height="178"> |

<h2>Как выглядит результат?</h2>
Результат выписывается в виде Latex файла, в котором присутствуют:
<ul>
 <li>Все стадии оптимизации дерева</li>
 <li>График изначальной функции и разложения в ряд Тейлора</li>
 <li>График изначальной функции и касательной к ней в точке 0</li>
</ul>
<p>Рассмотрим <b>пример</b> работы программы для функции f(x) = sin(x) + cos(x^2)</p>

<p>Процесс оптимизации:</p>

![image](https://github.com/SvetoCopy/Differentiator/assets/65361271/5679f521-c8f1-4b8b-852c-fb1ec5e888f0)
<p>Добавлены речевые связки, которые часто используются в математической литературе</p>

<p>Результат дифференцирования:</p>

![image](https://github.com/SvetoCopy/Differentiator/assets/65361271/7e790cc7-5f1f-4ecb-a2bd-800b4d456d3a)

<p>График функции и ряда Тейлора до 5 члена:</p>

![taylor](https://github.com/SvetoCopy/Differentiator/assets/65361271/7ecc60c4-da9b-4de9-b68e-d023f4050502)

<p>График функции и график касательной в точке 0:</p>

![tangent](https://github.com/SvetoCopy/Differentiator/assets/65361271/a28a13ae-1991-4592-a086-53861ff8f637)

