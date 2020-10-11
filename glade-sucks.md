Glade has a tendency to remove manually added properties it doesn't understand, which includes the property to reveal the bottom view switcher bar when on a mobile screen. Place this line:

`<property name="reveal" bind-source="viewSwitcherTitle" bind-property="title-visible" bind-flags="sync-create"/>`

in the viewSwitcherBar object under the following line:

`<property name="stack">stack</property>`
